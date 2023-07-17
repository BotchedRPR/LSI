//This file is a big preprocessor if statement, only here to not clog up the lsi.c file to keep that clean and organized.
//Include module header files that can autostart here, throw them into the preprocessor if statement in header declaration and in code that calls the show method. I think we can simplify this later.

#ifdef CONFIG_AUTOSTART_SIMPLE_DEBUG
#include "modules/debugmenu/debugmenu.h"
#endif

void autostart()
{
	#ifdef CONFIG_AUTOSTART_PANIC
		printf("Kernel panic inbound!\nSet CONFIG_AUTOSTART to something sensible next time ;)");
		return;
	#endif
	#ifdef CONFIG_AUTOSTART_SIMPLE_DEBUG
		show();
		return;
	#endif
}
