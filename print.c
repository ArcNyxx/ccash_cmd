#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "endpoint.h"
#include "arg.h"
#include "req.h"
#include "print.h"

static void print_exit(const char *fmt, ...);

static void
print_exit(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);
	exit(0);
}

void
print_res(const Args args, const Memory mem)
{
	int code = atoi(mem.str + 9), value;
	char *body = strstr(mem.str, "\r\n\r\n") + 4;

	switch (code) {
	default: break;
	case 401:
		print_exit("ccash_cmd: server: invalid username or password (%s)\n",
			args.auth);
		return;
	case 404:
		if (args.ep == &eps[10])
			print_exit("ccash_cmd: server: logs are disabled\n");
		else
			print_exit("ccash_cmd: server: user not found (%s)\n", args.name);
		return;
	case 409:
		print_exit("ccash_cmd: server: user already exists (%s)\n", args.name);
		return;
	case 204:
		print_exit("ccash_cmd: server: success\n");
		return;
	}

	/* only 200 OK remains, require specific function to print values */
	if (body[0] == '[') {
		unsigned long amount, time;
		char *to, *from;
		++body;

		printf("ccash_cmd: server:\n");
		while (sscanf(body, "{\"to\":\"%s\",\"from\":\"%s\","
			"\"amount\":%lu,\"time\":%lu},", to, from, &amount, &time) == 4)
			printf("\nsender: %s\nrecipient: %s\namount: %lu\ntime: %lu\n", to, from, amount, time);
	} else if (body[0] == '{') {
		int version, maxlog;
		char *retdel;

		value = sscanf(body, "{\"version\":%d,\"max_log\":%d,\"return_on_del\":\"%s\"}",
			&version, &maxlog, retdel);
		printf("ccash_cmd: server\nversion: %d\nmax_log : %d\n", version, maxlog);
		if (value == 3)
			printf("return_on_delete_account: %s\n", retdel);
		exit(0);
	} else {
		value = strtol(body, NULL, 10);
		if (args.ep == &eps[13])
			print_exit("ccash_cmd: server: (%d) users were pruned\n", value);
		else
			print_exit("ccash_cmd: server: the user's balance is now (%d)\n", value);
	}
}
