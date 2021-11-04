#ifndef ARGS_H
#define ARGS_H

#include "endpoints.h"

typedef struct args {
	const char *name, *passwd, *amount, *auth, *time, *server;
	const Endpoint *ep;
} Args;

void parse_args(Args *args, const char **argv);

#endif /* ARGS_H */
