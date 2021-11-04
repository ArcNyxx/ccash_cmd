#ifndef REQ_H
#define REQ_H

typedef struct memory {
        char *str;
        size_t len, alloc;
} Memory;

Memory request(const Args *args);

#endif /* REQ_H */
