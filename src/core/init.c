#include <console/core.h>
#include <console/font_default.h>
#include <console/fbcon.h>

void main()
{
	consolesetup();
	printc("lsi2: Hello World");
	while(1){};
}
