#include <stdio.h>
#include <sys/mount.h>
#include <sys/stat.h>

/*
 * For /sys/class/graphics/fb0/*
 */
int mount_sysfs(const char *target)
{
	struct stat st = {0};
	const char *source = "sysfs";
	const char *filesystemtype = "sysfs";
	unsigned long mountflags = 0;
	const void *data = NULL;

	if (stat(target, &st) == -1)
		if (mkdir(target, 0755) != 0)
			return -1;

	if (mount(source, target, filesystemtype, mountflags, data) != 0)
		return -1;

	return 0;
}

/*
 * Optional workaround for some Qualcomm/Exynos devices
 * that don't automatically refresh their framebuffer.
 * Run it either way. Doesn't cause harm.
 */
void msm_refresh()
{
	FILE *src = fopen("/sys/class/graphics/fb0/modes", "r");
	if (!src) {
		perror("Failed to open source file");
		return;
	}

	FILE *dst = fopen("/sys/class/graphics/fb0/mode", "w");
	if (!dst) {
		perror("Failed to open destination file");
		fclose(src);
		return;
	}

	char buffer[256];
	while (fgets(buffer, sizeof(buffer), src)) {
		fputs(buffer, dst);
	}

	fclose(src);
	fclose(dst);
}