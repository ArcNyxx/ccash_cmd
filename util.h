#define LENGTH(array) (sizeof(array) / sizeof(array[0]))

void die(const char *fmt, ...);
void die_perror(const char *err);
