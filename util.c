/*
 * ccash_cmd - command line interface for ccash servers
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void
die(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
}

void
die_perror(const char *err)
{
	perror(err);
	exit(1);
}
