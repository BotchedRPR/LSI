#include <console/fbcon.h>
#include <console/font_default.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static int curr_line = 0;

static void console_write(char* str)
{
	if(draw_string(10, (curr_line*FONT_Y)+16, str) == 1)
	{
		curr_line = 0;
		draw_string(10, 16, str);
	}

	if(str[strlen(str) - 1] == '\n')
		curr_line++;
}

void printc(const char* format, ...)
{
	// If it's larger than 2048, bad things will happen.
	// Please fix this stuff.
	char* str = malloc(8096 * sizeof(char));

	va_list va;
	va_start(va, format);
	vsprintf(str, format, va);
	va_end (va);

	console_write(str);
	free(str);
}

/*
 * consolesetup() - set everything up so that printc() can work.
 */
void consolesetup()
{
	// currently we only support fbcon
	fbcon_setup();
}