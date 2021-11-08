# ccash_cmd - command line interface for ccash servers
# Copyright (C) 2021 FearlessDoggo21
# see LICENCE file for licensing information

.POSIX:

include config.mk

SRC = ccash_cmd.c req.c arg.c util.c
HEAD = $(wildcard *.h)
OBJ = $(SRC:.c=.o)

all: options ccash_cmd

options:
	@echo ccash_cmd build options:
	@echo "CFLAGS  = $(CCASHCMDCFLAGS)"
	@echo "LDFLAGS = $(CCASHCMDLDFLAGS)"
	@echo "CC      = $(CC)"

config.h:
	cp config.def.h config.h

.c.o:
	$(CC) $(CCASHCMDCFLAGS) -c $<

$(OBJ): $(HEAD) config.h config.mk

ccash_cmd: $(OBJ)
	$(CC) -o $@ $(OBJ) $(CCASHCMDLDFLAGS)

clean:
	rm -f ccash_cmd $(OBJ) ccash_cmd-$(VERSION).tar.gz

dist: clean
	mkdir -p ccash_cmd-$(VERSION)
	cp -R LICENCE Makefile README config.mk \
		config.def.h $(SRC) $(HEAD) ccash_cmd-$(VERSION)
	tar -cf - ccash_cmd-$(VERSION) | gzip > ccash_cmd-$(VERSION).tar.gz
	rm -rf ccash_cmd-$(VERSION)

install: ccash_cmd
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f ccash_cmd $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/ccash_cmd

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/ccash_cmd

.PHONY: all options clean dist install uninstall
