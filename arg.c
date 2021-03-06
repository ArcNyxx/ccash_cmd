/* ccash_cmd - command line interface for ccash servers
 * Copyright (C) 2021-2022 FearlessDoggo21
 * see LICENCE file for licensing information */

#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "endpoint.h"
#include "arg.h"
#include "config.h"
#include "util.h"

typedef struct flag {
	const char *flag, *lflag;
} Flag;

static int is_base10(const char *num);

static const Flag flags[] = {
	{ "-n", "--name" },
	{ "-p", "--passwd" },
	{ "-c", "--amount" },
	{ "-t", "--time" },
	{ "-a", "--auth" },
	{ "-s", "--server" }
};

static int
is_base10(const char *num)
{
	for (int i = 0; num[i] != '\0'; ++i)
		if (!isdigit(num[i]))
			return 0;
	return 1;
}

void
parse_args(Args *args, const char **argv)
{
	size_t i, index;
	for (i = 0; i < LENGTH(eps); ++i)
		if (!strcmp(eps[i].cmd, argv[0])) {
			args->ep = &eps[i];
			break;
		}
	if (args->ep == NULL)
		die("ccash_cmd: [%s] is not a valid command\n", argv[0]);

	/* parse arguments into struct by casting struct to cstring array */
	for (i = 1; argv[i] != NULL && argv[i + 1] != NULL; i += 2) {
		size_t j;
		for (j = 0; j < LENGTH(flags); ++j)
			if (!strcmp(argv[i], flags[j].flag) || !strcmp(argv[i], flags[j].lflag)) {
				((const char **)args)[j] = argv[i + 1];
				break;
			}
		if (j == LENGTH(flags))
			die("ccash_cmd: [%s] is not a valid flag\n", argv[i]);
	}

	/* ensure required arguments passed */
	if (args->server == NULL)
		die("ccash_cmd: [--server] is a required flag\n");
	for (i = 0; i < 5; ++i) /* time is always optional */
		if (args->ep->info & 1 << i && ((const char **)args)[i] == NULL && i != 3)
			die("ccash_cmd: [%s] is a required flag\n", flags[i].lflag);

	/* handle for index arguments and verify */
	if (args->server[0] == '@') {
		index = strtol(&args->server[1], NULL, 10);
		if (index == 0 || index > LENGTH(servers))
			die("ccash_cmd: 1-based servers index out of range (%d)\n", index);
		args->server = servers[index - 1];
	}

	if (args->ep->info & REQ_NAME) {
		size_t j;
		for (j = 0; j < 17 && args->name[j] != '\0'; ++j)
			if (!isalnum(args->name[j]) && args->name[j] != '_')
				die("ccash_cmd: invalid character in name (%c)\n", args->name[j]);
		if (j < 3 || j > 16)
			die("ccash_cmd: name must be between 3 and 16 characters (%s)\n",
					args->name);
		if (args->ep->info & REQ_NAME_APPEND)
			return; /* skip unnecessary */
	}
	if (args->ep->info & REQ_AUTH) {
		if (args->auth[0] == '@') {
			index = strtol(&args->auth[1], NULL, 10) - 1;
			if (index == 0 || index > LENGTH(auth))
				die("ccash_cmd: 1-based auth index out of range (%d)\n", index);
			args->auth = auth[index - 1];
		}
		size_t j;
		for (j = 0; j < 16 && args->auth[j] != '\0' && args->auth[j] != ':'; ++j)
			if (!isalnum(args->auth[j]) && args->auth[j] != '_')
				die("ccash_cmd: invalid character in name (%c)\n", args->auth[j]);
		if (j < 3 || j > 16)
			die("ccash_cmd: name must be between 3 and 16 characters (%s)\n", args->auth);
		if (args->auth[j] != ':')
			die("ccash_cmd: auth name must be terminated with a colon (%s)\n", args->auth);
	}
	if (args->ep->info & REQ_AMOUNT && !is_base10(args->amount))
		die("ccash_cmd: amount must be an integer");
	if (args->ep->info & REQ_TIME &&
		args->time != NULL && !is_base10(args->time))
		die("ccash_cmd: time must be an integer");
}
