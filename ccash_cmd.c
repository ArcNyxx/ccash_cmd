#include "endpoint.h"
#include "arg.h"
#include "req.h"
#include "print.h"
#include "util.h"

int
main(int argc, const char **argv)
{
	if (argc < 4)
		die("ccash_cmd: a command and server must be supplied\n");

	Args args = { 0 };
	parse_args(&args, &argv[1]);
	print_res(args, request(&args));
}
