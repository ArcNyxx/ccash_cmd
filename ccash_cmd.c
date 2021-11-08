#include "endpoint.h"
#include "arg.h"
#include "req.h"
#include "print.h"
#include "util.h"

#include <stdio.h>
int
main(int argc, const char *argv[])
{
	if (argc < 4)
		die("ccash_cmd: a command and server must be supplied\n");

	Args args = { 0 };
	parse_args(&args, &argv[1]);

	Memory mem = request(&args);
	mem.str[mem.len] = '\0';
	printf("%s\n", mem.str);
	// print_res(args, request(&args));
}
