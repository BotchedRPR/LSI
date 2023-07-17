//This file is just a big if case, to not clog up the main debugmenu file
//I think this is efficient enough as it only goes to the parts that are defined
//in the config files.

#ifdef CONFIG_DEBUG_USER1
#include "modules/debugmenu/user/user1.h"
#endif
#ifdef CONFIG_DEBUG_USER2
#include "modules/debugmenu/user/user2.h"
#endif
#ifdef CONFIG_DEBUG_USER3
#include "modules/debugmenu/user/user3.h"
#endif
int launcher(char* module)
{
	if(module == "Bail out")
	{
		return 0;
	}
	#ifdef CONFIG_DEBUG_FILEBROWSER
	else if(module == "File browser")
	{
		return 1; //Placeholder for testing
	}	
	#endif
	#ifdef CONFIG_DEBUG_USER1
	else if(module == user1_menuitem_name)
	{
		return user1_main();
	}
	#endif
	#ifdef CONFIG_DEBUG_USER2
	else if(module == user2_menuitem_name)
	{
		return user2_main();
	}
	#endif
	#ifdef CONFIG_DEBUG_USER3
	else if(module == user3_menuitem_name)
	{
		return user3_main();
	}
	#endif
}
