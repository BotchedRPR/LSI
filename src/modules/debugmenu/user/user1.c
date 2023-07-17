//You can include anything, this will be ran like any other C program.
//
//Included demo is the classic Hello World. 
//
#include <stdio.h>

#include "modules/debugmenu/user/user1.h"

char* user1_menuitem_name = "user1.c";

int user1_main()
{
	printf("Hello world! (From user1.h)!");
	return 0;
}
