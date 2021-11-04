#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "endpoints.h"
#include "util.h"

static size_t
write_data(void *buf, size_t size, size_t nmemb, void *userp)
{

}

static char *
make_body(Args *args)
{

}

char *
request(Args *args)
{
	CURL *curl;
	struct curl_slist *slist = NULL;
	char *body;

	if (curl_global_init(CURL_GLOBAL_SSL))
		die("ccash_cmd: unable to initialise curl\n");
	if ((curl = curl_easy_init()) == NULL)
		die("ccash_cmd: unable to instantiate curl\n");

	if (args->auth != NULL) {
		curl_easy_setopt(curl, CURLOPT_USERPWD, args->auth);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	}

	if (args->ep->req & REQ_NAME_APPEND) {
		/* warning: discarding const qualifier */
		strcat(args->ep->ep, args->name);
	} else if ((body = make_body(args)) != NULL) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
		curl_slist_append(slist, "Content-Type: application/json");
	}
	curl_slist_append(slist, "Expect:");

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, methods[args->ep->info & 4]);
	curl_easy_setopt(curl, CURLOPT_URL, args->server);
	if (curl_easy_perform(curl) != CURLE_OK)
		die("ccash_cmd: unable to make request\n");

	if (body != NULL)
		free(body);
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}
