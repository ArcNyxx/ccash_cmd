#include "endpoint.h"
#include "req.h"
#include "print.h"

static void
get_info(const Memory mem, char **code, char **body)
{
	code = mem.str + 9, body = strstr(mem.str, "\r\n\r\n");
}

void
print_res(const Args args, const Memory mem)
{
	int code = atoi(mem.str + 9);
}
