# ccash_cmd - command line interface for ccash servers
# Copyright (C) 2021 FearlessDoggo21
# see LICENCE file for licensing information

VERSION = 1.1.0

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

WPROFILE = -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes \
-Wmissing-declarations -Wswitch-default -Wunreachable-code -Wcast-align \
-Wpointer-arith -Wbad-function-cast -Winline -Wundef -Wnested-externs \
-Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal
STD = -D_POSIX_C_SOURCE=200809L
LIB = -lcurl

CFLAGS = $(WPROFILE) $(STD) -Os -DVERSION=\"$(VERSION)\"
LDFLAGS = $(LIB)
