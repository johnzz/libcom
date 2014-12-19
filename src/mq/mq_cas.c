#define _GNU_SOURCE
#include <pthread.h>

#include <sched.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct msg msg_t;

static struct head  mq_head;

struct head {
    msg_t   *next;
    msg_t   *prev;
};

struct msg {
    unsigned long       itemnum;
    msg_t               *item; 
};


static int mq_push(void *it)
{
    msg_t       *msg;
    msg = malloc(sizeof(msg_t));

    do {
        
    }
}

void *mq_pop()
{
}

static int init_mq()
{
    mq_head.next = NULL;
    mq_head.prev = NULL;
}

