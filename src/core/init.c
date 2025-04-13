#include <console/core.h>
#include <console/fbcon.h>
#include <console/font_default.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>

void main()
{
	consolesetup();
	printc("lsi2: Hello World\n");
	while(1){}
}
