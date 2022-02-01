/* ccash_cmd - command line interface for ccash servers
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information */

typedef struct response {
	const char *head, *body;
} Response;

Response request(Args *args);
