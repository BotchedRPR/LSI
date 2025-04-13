#include <console/fbcon.h>
#include <console/font_default.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <unistd.h>

/* Accessed by both threads - "read-only" */
static int fb_data_size;

/* Accessed by both threads but only one at a time */
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;

/* Accessed by both threads - protected by mutexes */
static char *fb_noblit;

/* Accessed by only thread 2 */
static char *fb_blit;

/* pthread stuff */
pthread_t blit_thread;
static pthread_mutex_t noblit_mutex = PTHREAD_MUTEX_INITIALIZER;

static void fbcon_mknods()
{
	mode_t mode = S_IFCHR | 0666;
	dev_t dev = makedev(29, 0);
	mkdir("/dev", 0755);

	// For MSM
	mount_sysfs("/sys");

	mknod("/dev/fb0", mode, dev);
}

static int fbcon_open_fb()
{
	int fb;

	fb = open("/dev/fb0", O_RDWR);

	if (fb == -1) return fb;

	// Get screen info
	ioctl(fb, FBIOGET_FSCREENINFO, &finfo);
	ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);

	fb_data_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Allocate memory for both thread's arrays, map only one to fb mem
	// region
	fb_noblit = (char *)malloc(fb_data_size);
	fb_blit = mmap(0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb,
		       (off_t)0);

	// Since now, NO touching of fb_blit and {f,v}info on this thread.
	return 0;
}

static volatile int framebuffer_dirty = 0;

static void *blit_function(void *arg)
{
	while (1) {
		if (framebuffer_dirty)
		{
			pthread_mutex_lock(&noblit_mutex);
			memcpy(fb_blit, fb_noblit, fb_data_size);
			pthread_mutex_unlock(&noblit_mutex);
		}
		msm_refresh();
	}
}

void draw_pixel(int x, int y, uint32_t color)
{
	pthread_mutex_lock(&noblit_mutex);
	uint8_t *pixel_ptr = fb_noblit + y * finfo.line_length +
			     (x * (vinfo.bits_per_pixel / 8));

	switch (vinfo.bits_per_pixel) {
		case 32:
			*(uint32_t *)pixel_ptr = color;
			break;
		case 24:
			pixel_ptr[0] = color & 0xFF;
			pixel_ptr[1] = (color >> 8) & 0xFF;
			pixel_ptr[2] = (color >> 16) & 0xFF;
			break;
		case 16:
			*(uint16_t *)pixel_ptr = color;
			break;
		case 8:
			*pixel_ptr = color;
			break;
		default:
			// Unsupported bp
			break;
	}
	framebuffer_dirty = 1;
	pthread_mutex_unlock(&noblit_mutex);
}

void draw_char(int x, int y, char c, uint32_t fg_color, uint32_t bg_color)
{
	int index = c * (FONT_Y * 2);  // 2 ints per row

	for (int row = 0; row < FONT_Y; ++row) {
		uint32_t row_data_left = font[index + row * 2];
		uint32_t row_data_right = font[index + row * 2 + 1];

		// First half
		for (int col = 0; col < (FONT_X / 2); ++col) {
			uint32_t mask = 1 << col;
			uint32_t color =
			    (row_data_left & mask) ? fg_color : bg_color;
			draw_pixel(x + ((FONT_X / 2 - 1) - col), y + row,
				   color);
		}

		// Second half
		for (int col = 0; col < (FONT_X / 2); ++col) {
			uint32_t mask = 1 << col;
			uint32_t color =
			    (row_data_right & mask) ? fg_color : bg_color;
			draw_pixel(x + (FONT_X / 2 + (FONT_X / 2 - 1) - col),
				   y + row, color);
		}
	}
}

int draw_string(int x, int y, const char *str)
{
	if (y + (FONT_Y*2) >= vinfo.yres)
	{
		memset (fb_noblit, 0, fb_data_size);
		framebuffer_dirty = 1;
		return 1;
	}

	while (*str) {
		draw_char(x, y, *str, FONT_WHITE, FONT_BLACK);
		if(x+FONT_X >= vinfo.xres)
			return 2;
		x += FONT_X;  // Move right for next character
		str++;
	}
}

void fbcon_setup()
{
	/*
	 * We use threaded fbcon, so we need to do a bit of thinking here.
	 * Thread 1:
	 *  - run lsi
	 *  - setup fbcon data
	 *  - run console/core wrappers
	 *  - modify fb_noblit array
	 *
	 * Thread 2:
	 *  - copy fb_noblit array to fb_blit array
	 *  - refresh the framebuffer
	 */

	// assume that we don't have CONFIG_DEVTMPFS_MOUNT
	fbcon_mknods();

	// setup framebuffer info
	if (fbcon_open_fb() != 0) return;

	// Run framebuffer thread
	if (pthread_create(&blit_thread, NULL, blit_function, NULL) != 0)
		return;

}