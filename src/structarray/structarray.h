#ifndef __STRUCTARRAY__

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef int (*libcomm_structarray_handler_pt)(void *data);

typedef struct {
    char        *host;
    char        *agent;
    char        *ip;
    char        *port;
}libcomm_structarray_member_t;

typedef struct {
    char        *name;
    int         offset;
    libcomm_structarray_handler_pt  handler;
}libcomm_structarray_t;


#endif

