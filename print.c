/* ccash_cmd - command line interface for ccash servers
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "endpoint.h"
#include "arg.h"
#include "req.h"
#include "print.h"

static void print_log(const char *body);
static void print_prop(const char *body);

static void
print_log(const char *body)
{
	size_t amount, time;
	const char *sender, *recipient;
	char *endp;
	body += 1;

	printf("ccash_cmd: server: logs:\n");
	while (1) {
		if (body[0] == ']')
			return;

		sender = body + 7;
		body = strchr(body + 7, '"');
		recipient = body + 10;
		body = strchr(body + 10, '"');

		amount = strtoul(body + 11, &endp, 10);
		time = strtoul(body + 8, &endp, 10);
		body = endp + 2;

		printf("sender: %s\nrecipient: %s\namount: %lu\ntime: %lu\n",
			sender, recipient, amount, time);
	}
}

static void
print_prop(const char *body)
{
	size_t version, maxlog;
	char *endp;

	version = strtoul(body + 11, &endp, 10);
	maxlog = strtoul(endp + 11, &endp, 10);
	printf("ccash_cmd: server:\nversion: %lu\nmax_log: %lu\n", version, maxlog);

	if (endp[0] != '}') {
		endp += 18;
		printf("return_on_delete_account: %.*s\n",
			(int)(strchr(endp, '"') - endp), endp);
	}
}

void
print_res(const Args args, const Response res)
{
	switch(strtol(strchr(res.head, ' ') + 1, NULL, 10)) {
	default: break;
	case 204:
		printf("ccash_cmd: server: success\n");
		return;
	case 401:
		printf("ccash_cmd: server: invalid username or password (%s)\n",
			args.auth);
		return;
	case 404:
		if (args.ep == &eps[10])
			printf("ccash_cmd: server: logs are disabled\n");
		else
			printf("ccash_cmd: server: user not found (%s)\n", args.name);
		return;
	case 409:
		printf("ccash_cmd: server: user already exists (%s)\n", args.name);
		return;
	}

	/* only 200 OK remains, require specific function to print values */
	if (res.body[0] == '[') {
		print_log(res.body);
	} else if (res.body[0] == '{') {
		print_prop(res.body);
	} else {
		size_t value = strtol(res.body, NULL, 10);
		if (args.ep == &eps[13])
			printf("ccash_cmd: server: users pruned (%lu)\n", value);
		else
			printf("ccash_cmd: server: user's balance is now (%lu)\n", value);
	}
}
