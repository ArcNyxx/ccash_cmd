#ifndef ARGS_H
#define ARGS_H

typedef struct args {
	Endpoint *ep;
	char *name, *passwd, *auth, *server;
	long amount, time;
} Args;

typedef struct args_ptr {
	char *name, *passwd, *amount, *time, *user, *admin, *server;
} ArgsPtr;

void parse_args(Args *args, const char **argv);

#endif /* ARGS_H */
