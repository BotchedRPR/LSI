include .config

CC = gcc
CFLAGS = -static -o init -lcurses -ltinfo -I $(HEADERDIR)
ROOTDIR = $(shell pwd)
HEADERDIR = $(ROOTDIR)/include
EXTRACFILES = 
SRCDIR = $(ROOTDIR)/src

configparse:
	@if [ "$(CONFIG_SIMPLE_DEBUG)" = "y" ]; then $(eval EXTRACFILES+=$(SRCDIR)/modules/debugmenu/debugmenu.c $(SRCDIR)/modules/debugmenu/launcher.c) $(eval CFLAGS += -DCONFIG_SIMPLE_DEBUG) echo; fi
	@if [ "$(CONFIG_WAIT_FOR_DEVICES)" = "y" ]; then $(eval CFLAGS += -DWAIT_FOR_DEVICES) $(eval CFLAGS += -DDEVICE_WAIT_TIME=$(DEVICE_WAIT_TIME)) echo; fi
	@if [ "$(CONFIG_DEBUG_FILEBROWSER)" = "y" ]; then $(eval CFLAGS += -DCONFIG_DEBUG_FILEBROWSER) echo; fi
	@if [ "$(CONFIG_DEBUG_USER1)" = "y" ]; then $(eval CFLAGS += -DCONFIG_DEBUG_USER1) $(eval EXTRACFILES+=$(SRCDIR)/modules/debugmenu/user/user1.c) echo; fi
	@if [ "$(CONFIG_DEBUG_USER2)" = "y" ]; then $(eval CFLAGS += -DCONFIG_DEBUG_USER2) $(eval EXTRACFILES+=$(SRCDIR)/modules/debugmenu/user/user2.c) echo; fi
	@if [ "$(CONFIG_DEBUG_USER3)" = "y" ]; then $(eval CFLAGS += -DCONFIG_DEBUG_USER3) $(eval EXTRACFILES+=$(SRCDIR)/modules/debugmenu/user/user3.c) echo; fi

init: configparse
	$(CC) $(SRCDIR)/core/init.c $(EXTRACFILES) $(CFLAGS)

init_debug: configparse
	$(CC) $(SRCDIR)/core/init.c $(EXTRACFILES) $(CFLAGS) -g
