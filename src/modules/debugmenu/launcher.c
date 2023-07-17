//This file is just a big if case, to not clog up the main debugmenu file
//I think this is efficient enough as it only goes to the parts that are defined
//in the config files.

#include <string.h>

#ifdef CONFIG_DEBUG_USER1
#include "modules/debugmenu/user/user1.h"
#endif
#ifdef CONFIG_DEBUG_USER2
#include "modules/debugmenu/user/user2.h"
#endif
#ifdef CONFIG_DEBUG_USER3
#include "modules/debugmenu/user/user3.h"
#endif
#ifdef CONFIG_DEBUG_FILEBROWSER
#include "modules/filemanager/filemanager.h"
#include <stdio.h>
#endif
int launcher(char* module)
{
	if(strcmp(module, "Bail out") == 0)
	{
		return 0;
	}
	#ifdef CONFIG_DEBUG_FILEBROWSER
	else if(strcmp(module,"File browser") == 0)
	{
		int ret =  filemanager_show();
		printf("Filemanager module exited with code %i", ret);
	}	
	#endif
	#ifdef CONFIG_DEBUG_USER1
	else if(strcmp(module, user1_menuitem_name) == 0)
	{
		return user1_main();
	}
	#endif
	#ifdef CONFIG_DEBUG_USER2
	else if(strcmp(module, user2_menuitem_name) == 0)
	{
		return user2_main();
	}
	#endif
	#ifdef CONFIG_DEBUG_USER3
	else if(strcmp(module,user3_menuitem_name) == 0)
	{
		return user3_main();
	}
	#endif
	else
	{
		printf(module);
	}
}
