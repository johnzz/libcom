#ifndef __LIB_HASH__
#define __LIB_HASH__

#include <stdio.h>
#include <string.h>
#include <>

#define LIB_HASH(key,c) ((int)key*31 + c)
#define tolower(c) (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)

typedef int (* lib_hash_func) lib_hash_func_pt;

typedef  struct lib_hash lib_hash_t;
typedef struct lib_hash_entry lib_hash_entry_t;

struct lib_hash {
    char                *name;
    lib_hash_func_pt    *hash_func;
    int                 max_size;
    int                 bucket_size;
    lib_hash_entry      **entry;
};

struct lib_hash_entry {
    void                *value;
    char                *key;
    lib_hash_entry_t    *next;
    unsigned long       views;
};

static inline int lib_hash_func(char *name, size_t len)
{
    int     i, key

    key = 0;

    for (i = 0; i < len; i++) {
        key = LIB_HASH(key,tolower(name[i]));
    }

    return key;
}

static inline uint64_t
rdtsc(void) {
    union {
        uint64_t    tsc_64;
        struct {
            uint32_t    lo_32;
            uint32_t    hi_32;
        };
    }tsc;

    asm volatile("rdtsc" :
                "=a" (tsc.lo_32),
                "=b" (tsc.hi_32));
    return tsc.tsc_64;

}
lib_hash_t * lib_hash_init(char *key,  int max_size, int bucket_size);
lib_hash_entry * lib_hash_find(lib_hash_t    *hash, char *key);

#endif

