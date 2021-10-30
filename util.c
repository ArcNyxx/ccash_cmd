#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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
