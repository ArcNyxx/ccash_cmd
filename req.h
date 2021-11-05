typedef struct memory {
        char *str;
        unsigned long len, alloc;
} Memory;

Memory request(Args *args);
