typedef struct args {
        const char *name, *passwd, *amount, *time, *auth, *server;
        const Endpoint *ep;
} Args;

void parse_args(Args *args, const char **argv);
