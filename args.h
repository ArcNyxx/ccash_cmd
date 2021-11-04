#ifndef ARGS_H
#define ARGS_H

#ifndef ENDPOINT_H
#include "endpoints.h"
#endif /* ENDPOINT_H */

typedef struct args {
	const Endpoint *ep;
	const char *name, *passwd, *auth, *server;
	long amount, time;
} Args;

typedef struct args_ptr {
	char *name, *passwd, *amount, *time, *user, *admin, *server;
} ArgsPtr;

void parse_args(Args *args, const char **argv);

#endif /* ARGS_H */
