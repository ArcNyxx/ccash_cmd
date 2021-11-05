#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "args.h"
#include "config.h"
#include "def.h"
#include "util.h"

static void verify_auth(const char *auth);
static void verify_name(const char *name);

static void
verify_auth(const char *str)
{
	int i;
	for (i = 0; i < 16 && str[i] != '\0' && str[i] != ':'; ++i)
		if (!isalnum(str[i]) && str[i] != '_')
			die("ccash_cmd: invalid character in name (%c)\n", str[i]);
        if (i < 3 || i > 16)
                die("ccash_cmd: name must be between 3 and 16 characters (%s)\n", str);
	if (str[i] != ':')
		die("ccash_cmd: auth name must be terminated with a colon (%s)\n", str);
}

static void
verify_name(const char *name)
{
	int i;
	for (i = 0; i < 16 && name[i] != '\0'; ++i)
		if (!isalnum(name[i]) && name[i] != '_')
			die("ccash_cmd: invalid character in name (%c)\n", name[i]);
        if (i < 3 || i > 16)
                die("ccash_cmd: name must be between 3 and 16 characters (%s)\n", name);
}

static inline int
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
	unsigned int i, j, index;
	for (i = 0; i < LENGTH(eps); ++i)
		if (!strcmp(eps[i].cmd, argv[0])) {
			args->ep = &eps[i];
			break;
		}
	if (args->ep == NULL)
		die("ccash_cmd: [%s] is not a valid command\n", argv[0]);

	for (i = 1; argv[i + 1] != NULL && argv[i + 2] != NULL; i += 2) {
		for (j = 0; j < LENGTH(flags); ++j)
			if (!strcmp(argv[i], flags[j].flag) || !strcmp(argv[i], flags[j].lflag)) {
				((const char **)args)[j] = argv[i + 1];
				break;
			}
		if (j == LENGTH(flags))
			die("ccash_cmd: [%s] is not a valid flag\n", argv[i]);
	}

	if (args->server == NULL)
		die("ccash_cmd: [--server] is a required flag\n");
	for (i = 0; i < 5; ++i) /* time is always optional */
		if (args->ep->info & 1 << i && ((const char **)args)[i] == NULL && i != 3)
			die("ccash_cmd: [%s] is a required flag\n", flags[i].lflag);

	if (args->server[0] == '@') {
		index = strtol(&args->server[1], NULL, 10);
		if (index == 0 || index > LENGTH(servers))
			die("ccash_cmd: 1-based servers index out of range (%d)\n", index);
		args->server = servers[index - 1];
	}
	if (args->ep->info & REQ_NAME) {
		verify_name(args->name);
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
		verify_auth(args->auth);
	}
	if (args->ep->info & REQ_AMOUNT && !is_base10(args->amount))
		die("ccash_cmd: amount must be an integer");
	if (args->ep->info & REQ_TIME &&
		args->time != NULL && !is_base10(args->time))
		die("ccash_cmd: time must be an integer");
}
