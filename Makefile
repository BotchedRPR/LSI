include .config

CC = gcc
CFLAGS = -static -o init -lcurses -O2 -ltinfo -I $(HEADERDIR)
ROOTDIR = $(shell pwd)
HEADERDIR = $(ROOTDIR)/include
EXTRACFILES = 
SRCDIR = $(ROOTDIR)/src
getstartupmodule:
ifeq ($(CONFIG_AUTOSTART),panic)
	$(eval CFLAGS += -DCONFIG_AUTOSTART_PANIC)
else ifeq ($(CONFIG_AUTOSTART),simple_debug)
	$(eval CFLAGS += -DCONFIG_AUTOSTART_SIMPLE_DEBUG) $(eval EXTRACFILES+=$(SRCDIR)/core/autostart.c)
endif

configparse: getstartupmodule
ifeq ($(CONFIG_SIMPLE_DEBUG),y)
	$(eval EXTRACFILES+=$(SRCDIR)/modules/debugmenu/debugmenu.c $(SRCDIR)/modules/debugmenu/launcher.c) $(eval CFLAGS += -DCONFIG_SIMPLE_DEBUG)
endif
ifeq ($(CONFIG_WAIT_FOR_DEVICES),y)
	$(eval CFLAGS += -DWAIT_FOR_DEVICES) $(eval CFLAGS += -DDEVICE_WAIT_TIME=$(DEVICE_WAIT_TIME))
endif
ifeq ($(CONFIG_DEBUG_FILEBROWSER),y)
	$(eval CFLAGS += -DCONFIG_DEBUG_FILEBROWSER) $(eval EXTRACFILES+=$(SRCDIR)/modules/filemanager/filemanager.c)
endif
ifeq ($(CONFIG_DEBUG_USER1),y)
	$(eval CFLAGS += -DCONFIG_DEBUG_USER1) $(eval EXTRACFILES+=$(SRCDIR)/modules/debugmenu/user/user1.c)
endif
ifeq ($(CONFIG_DEBUG_USER2),y)
        $(eval CFLAGS += -DCONFIG_DEBUG_USER2) $(eval EXTRACFILES+=$(SRCDIR)/modules/debugmenu/user/user2.c)
endif
ifeq ($(CONFIG_DEBUG_USER3),y)
        $(eval CFLAGS += -DCONFIG_DEBUG_USER3) $(eval EXTRACFILES+=$(SRCDIR)/modules/debugmenu/user/user3.c)
endif
ifeq ($(CONFIG_EXIT_LOOP),y)
	$(eval CFLAGS += -DCONFIG_EXIT_LOOP)
endif

init: configparse
	$(CC) $(SRCDIR)/core/init.c $(EXTRACFILES) $(CFLAGS)

init_debug: configparse
	$(CC) $(SRCDIR)/core/init.c $(EXTRACFILES) $(CFLAGS) -g
