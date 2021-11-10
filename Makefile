# ccash_cmd - command line interface for ccash servers
# Copyright (C) 2021 FearlessDoggo21
# see LICENCE file for licensing information

.POSIX:

include config.mk

SRC = arg.c ccash_cmd.c endpoint.c print.c req.c util.c
HEAD = arg.h endpoint.h print.h req.h util.h
OBJ = $(SRC:.c=.o)

all: ccash_cmd

config.h:
	cp config.def.h config.h

$(OBJ): $(HEAD) config.h config.mk

.c.o:
	$(CC) $(CFLAGS) -c $<

ccash_cmd: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

clean:
	rm -rf ccash_cmd $(OBJ) ccash_cmd-$(VERSION).tar.gz

dist: clean
	mkdir -p ccash_cmd-$(VERSION)
	cp -r LICENCE README Makefile ccash_cmd.1 config.mk $(SRC) $(HEAD) \
		config.def.h ccash_cmd-$(VERSION)
	tar -cf ccash_cmd-$(VERSION).tar ccash_cmd-$(VERSION)
	gzip ccash_cmd-$(VERSION).tar
	rm -rf ccash_cmd-$(VERSION)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin $(DESTDIR)$(MANPREFIX)/man1
	cp -f ccash_cmd $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/ccash_cmd
	sed "s/VERSION/$(VERSION)/g" < ccash_cmd.1 > \
		$(DESTDIR)$(MANPREFIX)/man1/ccash_cmd.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/ccash_cmd.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/ccash_cmd \
		$(DESTDIR)$(MANPREFIX)/man1/ccash_cmd.1

options:
	@echo ccash_cmd build options
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"

# implementation defined
.PHONY: all clean dist install uninstall options
