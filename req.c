#include <curl/curl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "endpoint.h"
#include "arg.h"
#include "req.h"
#include "util.h"

static size_t write_data(void *buf, size_t size, size_t nmemb, void *data);
static char *make_body(Args *args);

static const char *methods[] = { "GET", "DELETE", "POST", "PATCH" };
static const char *tokens[] = {
        "\"name\":", "\"pass\":", "\"amount\":", "\"time\":"
};

static size_t
write_data(void *buf, size_t size, size_t nmemb, void *data)
{
	Memory *mem = (Memory *)data;
	if (mem->len + size * nmemb > mem->alloc) {
		mem->alloc = mem->len + size * nmemb + 4096;
		mem->alloc -= mem->alloc % 4096;
		if ((mem->str = realloc(mem->str, mem->alloc)) == NULL)
			die_perror("ccash_cmd: unable to allocate memory\n");
	}

	register char *write = mem->str + mem->len; /* prevents sequencing error */
	memcpy(write, buf, mem->len = size * nmemb);
	return size * nmemb;
}

static char *
make_body(Args *args)
{
	static char body[4096];
	int i, index = 0, len, tklen;
	for (i = 0; i < 4; ++i) {
		if (((char **)args)[i]) {
			len = strlen(((char **)args)[i]), tklen = i != 2 ? 7 : 9;
			memcpy(body + index, tokens[i], tklen);
			memcpy(body + index + tklen, ((char **)args)[i], len);
			body[len + tklen] = ',';
			index += len + tklen + 1;
		}
	}
	body[index - 1] = '}';
	return body;
}

Memory
request(Args *args)
{
	CURL *curl;
	struct curl_slist *slist = NULL;
	char *body;
	Memory mem = { .len = 0, .alloc = 4096 };

	if (curl_global_init(CURL_GLOBAL_SSL))
		die("ccash_cmd: unable to initialise curl\n");
	if ((curl = curl_easy_init()) == NULL)
		die("ccash_cmd: unable to instantiate curl\n");


	if (args->ep->info == (REQ_NAME & REQ_NAME_APPEND)) {
		/* warning: discarding const qualifier */
		strcat(args->ep->ep, args->name);
	} else {
		if (args->ep->info & 128 && (body = make_body(args)) != NULL) {
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
			curl_slist_append(slist, "Content-Type: application/json");
		}
		if (args->auth != NULL) {
			curl_easy_setopt(curl, CURLOPT_USERPWD, args->auth);
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
		}
	}
	curl_slist_append(slist, "Expect:");

	if ((mem.str = malloc(mem.alloc)) == NULL)
		die_perror("ccash_cmd: unable to allocate memory\n");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, methods[args->ep->info >> 6]);
	curl_easy_setopt(curl, CURLOPT_URL, args->server);
	if (curl_easy_perform(curl) != CURLE_OK)
		die("ccash_cmd: unable to make request\n");

	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return mem;
}
