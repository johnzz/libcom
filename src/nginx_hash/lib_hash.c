#include "lib_hash.h"

lib_hash_t * lib_hash_init(int max_size, int bucket_size)
{
    lib_hash_t      *hash;
    char            *name = "test_hash";
    int             retkey,i,mod_num;
    lib_hash_entry_t    *entry;

    hash = malloc(sizeof(lib_hash_t));
    if (hash == NULL) {
        fprintf(stderr,"malloc error!\n");
        return NULL;
    }

    hash->name = malloc(strlen(name)+1);
    strcpy(hash->name,name);
    hash->name[strlen(name)+1] = '\0';

    hash->hash_func = lib_hash_func;
    hash->max_size = max_size;
    hash->bucket_size = bucket_size;

    hash->entry = malloc(sizeof(lib_hash_entry_t *)*(max_size/bucket_size));
    if (hash->entry == NULL) {
        fprintf(stderr,"malloc entry failed!\n");
        return NULL;
    }

    for (i = 0; i < bucket_size; i++) {
        hash->entry[i] = NULL;
    }

    return hash;
}

int  lib_hash_insert(lib_hash_t *hash, lib_hash_entry_t *entry, char *key)
{

    int     i,retkey;

    retkey = hash->hash_func(key, strlen(key));
    entry->views = 0;
    entry->next = NULL;

    mod_num = retkey % bucket_size;
    if (hash->entry[mod_num]) {
        hash->entry[0][mod_num] = entry;
        entry->next = NULL;
    } else {
        hash->entry[0][mod_num] = entry;

    }
}
