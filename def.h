#ifndef DEF_H
#define DEF_H

/* critical that struct args, enum req, and flags agree on order */

typedef struct endpoint {
        const char *cmd, *ep, info;
} Endpoint;

typedef struct args {
	const char *name, *passwd, *amount, *time, *auth, *server;
	const Endpoint *ep;
} Args;

typedef struct flag {
        const char *flag, *lflag;
} Flag;

typedef struct memory {
	char *str;
	unsigned long len, alloc;
} Memory;

typedef struct string {
        const char *str;
        unsigned long len;
} String;

enum req {
        REQ_NONE = 0,
        REQ_NAME_APPEND = 32,

        REQ_NAME = 1,
        REQ_PASSWD = 2,
        REQ_AMOUNT = 4,
        REQ_TIME = 8, /* time always sent even though optional, earliest matters */
        REQ_AUTH = 16,

        REQ_METHOD_GET = 0,
        REQ_METHOD_DELETE = 64,
        REQ_METHOD_POST = 128,
        REQ_METHOD_PATCH = 128 + 64
};

static const Flag flags[] = {
        { "-n", "--name" },
        { "-p", "--passwd" },
        { "-c", "--amount" },
        { "-t", "--time" },
        { "-a", "--auth" },
        { "-s", "--server" }
};

#ifdef SRC_METHODS_TOKENS
static const char *methods[] = {
        "GET", "POST", "PATCH", "DELETE"
};

#define MKSTRING(string) { string, sizeof(string) - 1 }
static const String tokens[] = {
        MKSTRING("\"name\":"),
        MKSTRING("\"pass\":"),
        MKSTRING("\"amount\":"),
        MKSTRING("\"time\":")
};
#endif /* SRC_METHODS_TOKENS */

/* extra room for name append, static so initialised to 0 */
#define DEFSTRING(name, string, extra) \
        static char name[sizeof(string) + extra] = string
DEFSTRING(get_bal_endpoint, "user/balance?name=", 16);
DEFSTRING(verify_user_endpoint, "user/exists?name=", 16);

static const Endpoint eps[] = {
        { "properties", "properties", REQ_METHOD_GET },
        { "close", "admin/shutdown", REQ_AUTH | REQ_METHOD_POST },

        /* bal ctl */
        { "get_bal", get_bal_endpoint, REQ_NAME | REQ_NAME_APPEND | REQ_METHOD_GET },
        { "send_funds", "user/transfer",
                REQ_NAME | REQ_AMOUNT | REQ_AUTH | REQ_METHOD_POST },
        { "set_bal", "admin/set_balance",
                REQ_NAME | REQ_AMOUNT | REQ_AUTH | REQ_METHOD_PATCH },
        { "impact_bal", "admin/impact_balance",
                REQ_NAME | REQ_AMOUNT | REQ_AUTH | REQ_METHOD_POST },

        /* user ctl */
        { "add_user", "user/register", REQ_NAME | REQ_PASSWD | REQ_METHOD_POST },
        { "del_user", "user/delete", REQ_AUTH | REQ_METHOD_DELETE },
        { "admin_add_user", "admin/user/register",
                REQ_NAME | REQ_PASSWD | REQ_AMOUNT | REQ_AUTH | REQ_METHOD_DELETE },
        { "admin_del_user", "admin/user/delete",
                REQ_NAME | REQ_AUTH | REQ_METHOD_DELETE },

        { "get_log", "user/log", REQ_AUTH | REQ_METHOD_GET },

        { "verify_user", verify_user_endpoint,
                REQ_NAME | REQ_NAME_APPEND | REQ_METHOD_GET },
        { "admin_verify_user", "admin/verify_account", REQ_AUTH | REQ_METHOD_POST },
        { "prune", "admin/prune_users",
                REQ_AMOUNT | REQ_TIME | REQ_AUTH | REQ_METHOD_POST },

        /* passwd ctl */
        { "change_passwd", "user/change_password",
                REQ_PASSWD | REQ_AUTH | REQ_METHOD_PATCH },
        { "admin_change_passwd", "admin/user/change_password",
                REQ_NAME | REQ_PASSWD | REQ_AUTH | REQ_METHOD_PATCH },
        { "verify_passwd", "user/verify_password", REQ_AUTH | REQ_METHOD_POST }
};

#endif /* DEF_H */
