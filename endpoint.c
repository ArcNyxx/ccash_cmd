#include "endpoint.h"

/* extra room for name append, static so initialised to 0 */
#define STREXTRA(name, string, extra) name[sizeof(string) + extra] = string
static char STREXTRA(get_bal_endpoint, "user/balance?name=", 16);
static char STREXTRA(verify_user_endpoint, "user/exists?name=", 16);
const Endpoint eps[] = {
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
