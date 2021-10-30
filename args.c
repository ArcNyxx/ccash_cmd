#include <ctype.h>

#include "args.h"
#include "endpoints.h"
#include "util.h"

static void copy_auth(char **dest, char *src);

static void
copy_auth(char **dest, char *src, char **auth, unsigned long len)
{
	if (src[0] == '@') {
		int index = atoi(src);
		if (index == 0 || (index) > len)
			die("ccash_cmd: 1-based auth index out of range (%d)\n", index);
		*dest = auth[index - 1];
	} else {
		*dest = src;
	}

	int i;
	for (i = 0; i < 16 && *dest[i] != '\0' && *dest[i] != ':'; ++i)
		if (!isalnum(*dest[i]) && *dest[i] != '_')
			die("ccash_cmd: invalid character in username (%c)\n", *dest[i]);
	if (i < 3 || i > 16)
		die("ccash_cmd: username must be between 3 and 16 characters\n");
	if (*dest[i] != ':')
		die("ccash_cmd: password expected after username\n");
}

void
parse_args(Args *args, const char **argv)
{
	int i, j;
	for (i = 0; i < LENGTH(eps); ++i)
		if (!strcmp(eps[i].cmd, argv[0])) {
			args->ep = &eps[i];
			break;
		}
	if (args->ep == NULL)
		die("ccash_cmd: %s is not a valid command\n", argv[0]);

	/* parse flags into (char *) struct */
	ArgsPtr args_ptr = { 0 };
	for (i = 1; argv[i + 1] != NULL && argv[i + 2] != NULL; i += 2) {
		for (j = 0; j < LENGTH(flags); ++j)
			if (!strcmp(argv[i], flags[j].flag) || !strcmp(argv[i], flags[j].lflag)) {
				*(&args_ptr + j * sizeof(char *)) = argv[i + 1];
				break;
			}
		if (j == LENGTH(flags))
			die("ccash_cmd: %s is not a valid flag\n", argv[i]);
	}

	if (args_ptr.server[0] == '@') {
		int index = atoi(src);
		if (index == 0 || index > LENGTH(servers))
			die("ccash_cmd: 1-based servers index out of range (%d)\n", index);
		args->server = servers[index - 1];
	} else {
		args->server = args_ptr.server;
	}

	/* further parse flags but into respective data types */
	if (args->ep.req & REQ_NAME) {
		if (args_ptr.name == NULL)
			die("ccash_cmd: %s command requires name parameter\n", argv[0]);
		args->name = args_ptr.name;
		if (args->ep.req & REQ_NAME_APPEND)
			return; /* skip rest, unnecessary */
	}
	if (args->ep.req & REQ_PASSWD) {
		if (args_ptr.amount == NULL)
			die("ccash_cmd: %s command requires passwd parameter\n", argv[0]);
		args->passwd = args_ptr.amount;
	}
	if (args->ep.req & REQ_AMOUNT) {
		if (args_ptr.amount == NULL)
			die("ccash_cmd: %s command requires amount parameter\n", argv[0]);
		args->amount = atol(args_ptr.amount); /* no handling for different ints */
		if (args->amount == 0)
			die("ccash_cmd: amount parameter must be a non-zero number\n");
	}
	if (args->ep.req & REQ_TIME)
		if (!(args->endpoing.ep.req & REQ_TIME_OPTIONAL) && args_ptr.time == NULL) {
			die("ccash_cmd: %s command requires time parameter\n", argv[0]);
		} else if (args_ptr.time != NULL) {
			args->time = atol(args_ptr.time);
			if (args->time == 0 && args_ptr.time[0] != '0')
				die("ccash_cmd: time parameter must be a number\n");
		}

	if (args->ep.req & REQ_USER_AUTH)
		if (args_ptr.user != NULL)
			copy_auth(&args->auth, args_ptr.user, user_auth, LENGTH(user_auth));
		else if (args_ptr.admin != NULL)
			copy_auth(&args->auth, args_ptr.admin, admin_auth, LENGTH(admin_auth));
		else
			die("ccash_cmd: %s command requires user authentication\n", argv[0]);
	else if (args->ep.req & REQ_ADMIN_AUTH)
		if (args_ptr.admin != NULL)
			copy_auth(&args->auth, args_ptr.admin, admin_auth, LEGNTH(admin_auth));
		else
			die("ccash_cmd: %s command requires admin authentication\n", argv[0]);
	return;
}
