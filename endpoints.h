#ifndef ENDPOINTS_H
#define ENDPOINTS_H

typedef struct endpoint {
	const char *cmd, *ep, req, info;
} Endpoint;

typedef struct flag {
	const char *flag, *lflag;
} Flag;

enum req {
	REQ_NONE = 0,

	REQ_NAME = 1,
	REQ_PASSWD = 2,
	REQ_AMOUNT = 4,
	REQ_TIME = 8,

	REQ_USER_AUTH = 16,
	REQ_ADMIN_AUTH = 32,

	REQ_NAME_APPEND = 64,
	REQ_TIME_OPTIONAL = 128,
};

enum info {
	INFO_METHOD_GET = 0,
	INFO_METHOD_POST = 1
	INFO_METHOD_PATCH = 2,
	INFO_METHOD_DELETE = 3,

	INFO_RET_OK = 4,
	INFO_RET_NO_CONTENT = 5
};

static const char *methods[] = {
	"GET", "POST", "PATCH", "DELETE"
};

static const Flag flags[] = {
	{ "-n", "--name" },
	{ "-p", "--passwd" },
	{ "-c", "--amount" },
	{ "-t", "--time" },
	{ "-u", "--user" },
	{ "-a", "--admin" }
};

static const Endpoint eps[] = {
	{ "properties", "properties",
		REQ_NONE,
		INFO_RET_OK | INFO_METHOD_GET },
	{ "close", "admin/shutdown",
		REQ_ADMIN_AUTH,
		INFO_RET_NO_CONTENT | INFO_METHOD_POST },

	/* bal ctl */
	{ "get_bal", "user/balance?name=",
		REQ_NAME | REQ_NAME_APPEND,
		INFO_RET_OK | INFO_METHOD_GET },
	{ "send_funds", "user/transfer",
		REQ_NAME | REQ_AMOUNT | REQ_USER_AUTH,
		INFO_RET_OK | INFO_METHOD_POST },
	{ "set_bal", "admin/set_balance",
		REQ_NAME | REQ_AMOUNT | REQ_ADMIN_AUTH,
		INFO_RET_NO_CONTENT | INFO_METHOD_PATCH },
	{ "impact_bal", "admin/impact_balance",
		REQ_NAME | REQ_AMOUNT | REQ_ADMIN_AUTH,
		INFO_RET_OK | INFO_METHOD_POST },

	/* user ctl */
	{ "add_user", "user/register",
		REQ_NAME | REQ_PASSWD,
		INFO_RET_NO_CONTENT | INFO_METHOD_POST },
	{ "del_user", "user/delete",
		REQ_USER_AUTH,
		INFO_RET_NO_CONTENT | INFO_METHOD_DELETE },
	{ "admin_add_user", "admin/user/register",
		REQ_NAME | REQ_PASSWD | REQ_AMOUNT | REQ_ADMIN_AUTH,
		INFO_RET_NO_CONTENT | INFO_METHOD_DELETE },
	{ "admin_del_user", "admin/user/delete",
		REQ_NAME | REQ_ADMIN_AUTH,
		INFO_RET_NO_CONTENT | INFO_METHOD_DELETE },

	{ "get_log", "user/log",
		REQ_USER_AUTH,
		INFO_RET_OK | INFO_METHOD_GET },
	
	{ "verify_user", "user/exists?name=",
		REQ_NAME | REQ_NAME_APPEND,
		INFO_RET_NO_CONTENT | INFO_METHOD_GET },
	{ "admin_verify_user", "admin/verify_account",
		REQ_ADMIN_AUTH,
		INFO_RET_NO_CONTENT | INFO_METHOD_POST },
	{ "prune", "admin/prune_users",
		REQ_AMOUNT | REQ_TIME | REQ_TIME_OPTIONAL | REQ_ADMIN_AUTH,
		INFO_RET_OK | INFO_METHOD_POST },

	/* passwd ctl */
	{ "change_passwd", "user/change_password",
		REQ_PASSWD | REQ_USER_AUTH,
		INFO_RET_NO_CONTENT | INFO_METHOD_PATCH },
	{ "admin_change_passwd", "admin/user/change_password",
		REQ_NAME | REQ_PASSWD | REQ_ADMIN_AUTH,
		INFO_RET_NO_CONTENT | INFO_METHOD_PATCH },
	{ "verify_passwd", "user/verify_password",
		REQ_USER_AUTH,
		INFO_RET_NO_CONTENT | INFO_METHOD_POST },
};

#endif /* ENDPOINTS_H */
