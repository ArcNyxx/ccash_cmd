#include <stdio.h>

#include "args.h"
#include "util.h"

int
main(int argc, const char *argv[])
{
	if (argc < 4)
		die("ccash_cmd: a command and server must be supplied");

	Args args = { 0 };
	parse_args(&args, &argv[1]);


}
