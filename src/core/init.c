//Linux Simple Init
//For debugging
//Have fun!
//BotchedRPR - 2023
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>

#ifdef CONFIG_SIMPLE_DEBUG
#include "modules/debugmenu/debugmenu.h"
#endif
void consolesetup()
{
    int onefd = open("/dev/console", O_RDONLY, 0);
    dup2(onefd, 0); // stdin
    int twofd = open("/dev/console", O_RDWR, 0);
    dup2(twofd, 1); // stdout
    dup2(twofd, 2); // stderr

    if (onefd > 2) close(onefd);
    if (twofd > 2) close(twofd);

}
int main()
{
	consolesetup();
	printf("-----LSI version 1-----\n");
	printf("----Welcome to LSI!----\n");
#ifdef WAIT_FOR_DEVICES
	printf("----Waiting for USB----\n");
	sleep(DEVICE_WAIT_TIME);
#endif
#ifdef CONFIG_SIMPLE_DEBUG
	show();
#endif
	printf("Bailing out. Thanks for using LSI.\n");
	while(1);	
	return 1;
}
