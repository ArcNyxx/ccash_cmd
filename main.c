#include <stdio.h>

#include "args.h"

int
main(int argc, char *argv[])
{
	Args args = { 0 };
	parse_args(args, &argv[1]);


}
