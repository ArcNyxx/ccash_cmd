/* ccash_cmd - command line interface for ccash servers
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information */

typedef struct args {
        const char *name, *passwd, *amount, *time, *auth, *server;
        const Endpoint *ep;
} Args;

void parse_args(Args *args, const char **argv);
