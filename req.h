typedef struct response {
	const char *head, *body;
} Response;

Response request(Args *args);
