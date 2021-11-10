typedef struct endpoint {
        const char *cmd, *ep;
	const unsigned char info;
} Endpoint;

enum req {
        REQ_NONE = 0,
        REQ_NAME_APPEND = 32,

        REQ_NAME = 1,
        REQ_PASSWD = 2,
        REQ_AMOUNT = 4,
        REQ_TIME = 8,
        REQ_AUTH = 16,

        REQ_METHOD_GET = 0,
        REQ_METHOD_DELETE = 64,
        REQ_METHOD_POST = 128,
        REQ_METHOD_PATCH = 128 + 64
};

extern const Endpoint eps[17];
