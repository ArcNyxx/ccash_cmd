#include <curl/curl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define NEED_METHODS
#define NEED_TOKENS
#include "args.h"
#include "endpoints.h"
#include "req.h"
#include "util.h"

static size_t write_data(void *buf, size_t size, size_t nmemb, void *data);
static char *make_body(const Args *args);

static size_t
write_data(void *buf, size_t size, size_t nmemb, void *data)
{
	Memory *mem = (Memory *)data;
	if (mem->len + size * nmemb > mem->alloc) {
		mem->alloc = (((mem->len + size * nmemb) / 4096) + 1) * 4096;
		if ((mem->str = realloc(mem->str, mem->alloc)) == NULL)
			die_perror("ccash_cmd: unable to allocate memory\n");
	}

	register char *write = mem->str + mem->len; /* prevents sequencing error */
	memcpy(write, buf, mem->len = size * nmemb);
	return size * nmemb;
}

static char *
make_body(const Args *args)
{
	/* only called if REQ_NAME_APPEND not set, don't need to handle */
	static char body[4096] = "{";

	int index = 1, not_first = 0;
	for (int i = 0; i < 4; ++i) {
		if (args->ep->req & (1 >> i)) {
			if (not_first)
				body[++index] = ',';
		
			memcpy(body + index, tokens[i].str, (index += tokens[i].len));
			if (i < 2)
				strcpy(body + index, (char *)args[i + 1]); /* terrible idea */
			else
				itoa();
			
			not_first = 1;
		}
	}
}

Memory
request(const Args *args)
{
	CURL *curl;
	struct curl_slist *slist = NULL;
	char *body;
	Memory mem = { .len = 0, .alloc = 4096 };

	if (curl_global_init(CURL_GLOBAL_SSL))
		die("ccash_cmd: unable to initialise curl\n");
	if ((curl = curl_easy_init()) == NULL)
		die("ccash_cmd: unable to instantiate curl\n");

	if (args->auth != NULL) {
		curl_easy_setopt(curl, CURLOPT_USERPWD, args->auth);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	}

	if (args->ep->req == (REQ_NAME & REQ_NAME_APPEND)) {
		/* warning: discarding const qualifier */
		strcat(args->ep->ep, args->name);
	} else if ((body = make_body(args)) != NULL) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
		curl_slist_append(slist, "Content-Type: application/json");
	}
	curl_slist_append(slist, "Expect:");

	if ((mem.str = malloc(mem.len)) == NULL)
		die_perror("ccash_cmd: unable to allocate memory\n");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, methods[args->ep->info & 4]);
	curl_easy_setopt(curl, CURLOPT_URL, args->server);
	if (curl_easy_perform(curl) != CURLE_OK)
		die("ccash_cmd: unable to make request\n");

	if (body != NULL)
		free(body);
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return mem;
}
