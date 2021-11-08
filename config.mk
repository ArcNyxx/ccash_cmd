# ccash_cmd - command line interface for ccash servers
# Copyright (C) 2021 FearlessDoggo21
# see LICENCE file for licensing information

# ccash_cmd version
VERSION = 0.0.0-indev

# paths
PREFIX = /usr/local

# includes and libs
LIBS = -lcurl

# flags
WPROFILE = -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes \
-Wmissing-declarations -Wswitch-default -Wunreachable-code -Wcast-align \
-Wpointer-arith -Wbad-function-cast -Winline -Wundef -Wnested-externs \
-Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal
CCASHCMDCPPFLAGS = -DVERSION=\"$(VERSION)\" -D_XOPEN_SOURCE=600 $(WPROFILE) -g
CCASHCMDCFLAGS = $(CCASHCMDCPPFLAGS) $(CPPFLAGS) $(CFLAGS)
CCASHCMDLDFLAGS = $(LIBS) $(LDFLAGS)
