#define _GNU_SOURCE
#include <pthread.h>

#include <sched.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct item {
    unsigned long       itemnum;
}item_t;

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


static volatile unsigned long  head;
static volatile unsigned long tail;
static volatile unsigned long  pop_cycles;
static volatile unsigned long  push_cycles;

static volatile unsigned long pop_cnts = 0;
static volatile unsigned long push_cnts = 0;

static int  pop_quit = 0;
pthread_mutex_t mutex;


#define MAX_ITEM_NUM 10000000
#define PUSH_NUM    MAX_ITEM_NUM
#define POP_NUM     MAX_ITEM_NUM
#define MAX_THREAD_NUM 2

static void *vq[MAX_ITEM_NUM];

item_t      *item_head;

int vq_push(void *it)
{
    pthread_mutex_lock(&mutex);
    if((unsigned long)(head + 1) == tail) { 
            pthread_mutex_unlock(&mutex);
            return 1;
        }
    vq[head ++] = it;
    //printf("vq[]: %lu\n",((item_t *)(it))->itemnum);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void *vq_pop()
{
    pthread_mutex_lock(&mutex);
    int tmp = 0;
    if(tail == head) {
            pthread_mutex_unlock(&mutex);
            return 0;
        }
    tmp = tail++;
    pthread_mutex_unlock(&mutex);
    return vq[tmp];
}

void *vq_peek()
{
    if (tail == head) return 0;
    return vq[tail ++];
}

static void* push_loop(void *param)
{
    int ret;
    unsigned long   num;

    uint64_t    start;

    start = rdtsc();
    for(num=0; num < PUSH_NUM; num++) {
        item_head[num].itemnum = num;
        ret = vq_push((void *)(&item_head[num]));
        if (ret == 1) {
            printf("push item error!\n");
        }
    }

    push_cycles += rdtsc() - start;
    pop_quit = 1;
}

static void * pop_loop(void *param)
{
    int ret;
    unsigned long   num;
    item_t      *pop;
    uint64_t    start;

    start = rdtsc();
    //for(num=0; num < POP_NUM; num++) {
    while (!pop_quit) {
        pop = vq_pop();
        pop_cnts++;
    }

    pop_cycles += rdtsc() - start;
}

static int setaffinity(pthread_t *pt, int core_id)
{
    cpu_set_t       cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    if (pthread_setaffinity_np(*pt, sizeof(cpu_set_t), &cpuset)) {
        return -1;
    }
}

int main()
{
    int     thread_num=0,ret;
    pthread_t   thread_id[2];
    void *tret;

    item_head = malloc(PUSH_NUM*sizeof(item_t));


    pthread_attr_t  attr;
    pthread_attr_init(&attr);

    int *s = (int *)malloc(sizeof(int));
    *s = 0;

    if ((ret = pthread_create(&thread_id[0], &attr, push_loop, s)) != 0) {
        fprintf(stderr,"Can't create thread :%s\n",strerror(ret));
        return -1;
    }

    ret = setaffinity(&thread_id[0],0);
    if (ret < 0) {
        printf("setaffinity error!");
        return -1;
    }

    printf("ret %d thread_num:%d\n",ret,thread_num);

    if ((ret = pthread_create(&thread_id[1], &attr, pop_loop, s)) != 0) {
        fprintf(stderr,"Can't create thread :%s\n",strerror(ret));
        return -1;
    }

    ret = setaffinity(&thread_id[1],1);
    if (ret < 0) {
        printf("setaffinity error!");
        return -1;
    }

    for (thread_num=0; thread_num < MAX_THREAD_NUM; thread_num++) {
        printf("wait thread_num:%d\n",thread_num);
        pthread_join(thread_id[thread_num], NULL);
    }

    printf("push_cycles: %lu, avg: %lu\n",push_cycles, push_cycles/PUSH_NUM);
    printf("pop_cycles: %lu, avg: %lu, pop_cnts: %lu\n",pop_cycles, pop_cycles/pop_cnts, pop_cnts);
}
