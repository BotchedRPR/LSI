include .config

CC = gcc
CFLAGS = -static -o init -lcurses -ltinfo -I $(HEADERDIR)
ROOTDIR = $(shell pwd)
HEADERDIR = $(ROOTDIR)/include
EXTRACFILES = 
SRCDIR = $(ROOTDIR)/src

configparse:
	if [ "$(CONFIG_SIMPLE_DEBUG)" = "y" ]; then $(eval EXTRACFILES+="$(SRCDIR)/modules/debugmenu/debugmenu.c") $(eval CFLAGS += -DCONFIG_SIMPLE_DEBUG) echo; fi
	if [ "$(CONFIG_WAIT_FOR_DEVICES)" = "y" ]; then $(eval CFLAGS += -DWAIT_FOR_DEVICES) $(eval CFLAGS += -DDEVICE_WAIT_TIME=$(DEVICE_WAIT_TIME)) echo; fi
init: configparse
	$(CC) $(SRCDIR)/core/init.c $(EXTRACFILES) $(CFLAGS)
