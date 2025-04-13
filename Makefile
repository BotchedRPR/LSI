CC = $(CROSS_COMPILE)gcc
CFLAGS = -static -O2 -I $(HEADERDIR)
ROOTDIR = $(shell pwd)
HEADERDIR = $(ROOTDIR)/include
EXTRACFILES =
SRCDIR = $(ROOTDIR)/src

init: out
	$(CC) $(SRCDIR)/core/init.c $(SRCDIR)/console/core.c \
	$(SRCDIR)/console/fbcon.c  $(SRCDIR)/console/fbcon_msm.c \
	$(EXTRACFILES) $(CFLAGS) -o out/init

out:
	mkdir $@
