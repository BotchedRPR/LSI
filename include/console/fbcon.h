#ifndef CONSOLE_FBCON_H
#define CONSOLE_FBCON_H

/* macros */
#define FONT_BLACK 0xFF000000
#define FONT_GRAY 0xFF808080
#define FONT_WHITE 0xFFFFFFFF
#define FONT_RED 0xFFFF0000
#define FONT_GREEN 0xFF00FF00
#define FONT_BLUE 0xFF0000FF
#define FONT_YELLOW 0xFFFFFF00
#define FONT_ORANGE 0xFFFFA000

/* fbcon.c */
void fbcon_setup();
int draw_string(int x, int y, const char *str);

/* fbcon_msm.c */
int mount_sysfs(const char *target);
void msm_refresh();

#endif // CONSOLE_FBCON_H