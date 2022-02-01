/*
 * ccash_cmd - command line interface for ccash servers
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information
 */

#include <curl/curl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "endpoint.h"
#include "arg.h"
#include "req.h"
#include "util.h"

typedef struct memory {
        char *str;
        unsigned long len, alloc;
} Memory;

static char *make_body(Args *args);
static size_t write_data(void *buf, size_t size, size_t nmemb, void *data);

static const char *methods[] = { "GET", "DELETE", "POST", "PATCH" };
static const char *tokens[] = {
        "\"name\":\"", "\"pass\":\"", "\"amount\":", "\"time\":"
};

static char *
make_body(Args *args)
{
	static char body[512] = "{";
	int index = 1, len;
	for (int i = 0; i < 4; ++i) {
		if (((char **)args)[i] == NULL)
			continue;
		memcpy(&body[index], tokens[i], len = strlen(tokens[i]));
		index += len;
		memcpy(&body[index], ((char **)args)[i], len = strlen(((char **)args)[i]));
		index += len;
		if (i < 2)
			body[index++] = '"';
		body[index++] = ',';
	}

	body[index - 1] = '}';
	body[index] = '\0';
	return body;
}

static size_t
write_data(void *buf, size_t size, size_t nmemb, void *data)
{
	Memory *mem = (Memory *)data;
	if (mem->len + size * nmemb >= mem->alloc)
		if ((mem->str = realloc(mem->str,
			(mem->alloc += size * nmemb + 2048))) == NULL)
			die("ccash_cmd: unable to allocate memory");

	char *write = mem->str + mem->len; /* prevents sequencing error */
	memcpy(write, buf, mem->len += size * nmemb);
	return size * nmemb;
}

Response
request(Args *args)
{
	CURL *curl;
	struct curl_slist *slist = NULL;
	if (curl_global_init(CURL_GLOBAL_SSL))
		die("ccash_cmd: unable to initialise curl\n");
	if ((curl = curl_easy_init()) == NULL)
		die("ccash_cmd: unable to instantiate curl\n");

	if (args->ep->info == (REQ_NAME | REQ_NAME_APPEND)) {
		/* no need to get auth or body */
		strcat(args->ep->ep, args->name);
	} else {
		/* set http verb, make body if PUT or POST, set auth */
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, methods[args->ep->info >> 6]);
		if (args->ep->info & 128) {
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, make_body(args));
			slist = curl_slist_append(slist, "Content-Type: application/json");
		}
		if (args->auth != NULL) {
			curl_easy_setopt(curl, CURLOPT_USERPWD, args->auth);
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
		}
	}
	slist = curl_slist_append(slist, "Expect:");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	Memory body = { .len = 0, .alloc = 4096 },
		head = { .len = 0, .alloc = 4096 };
	if ((body.str = malloc(4096)) == NULL || (head.str = malloc(4096)) == NULL)
		die("ccash_cmd: unable to allocate memory");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &head);

	/* append the endpoint to the server, allocate >maximum endpoint length */
	char *server;
	unsigned long servlen = strlen(args->server);
	if ((server = malloc(servlen + 50)) == NULL)
		die("ccash_cmd: unable to allocate memory");

	memcpy(server, args->server, servlen);
	if (server[servlen - 1] != '/')
		server[servlen++] = '/';

	/* properties are version independent */
	if (args->ep == eps) {
		memcpy(server + servlen, "api/", 4);
		servlen += 4;
	} else {
		memcpy(server + servlen, "api/v1/", 7);
		servlen += 7;
	}
	strcpy(server + servlen, args->ep->ep);

	curl_easy_setopt(curl, CURLOPT_URL, server);
	if (curl_easy_perform(curl) != CURLE_OK)
		die("ccash_cmd: unable to make request\n");

	/* null terminate string */
	body.str[body.len] = '\0';

	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return (Response){ head.str, body.str };
}
