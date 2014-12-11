#include <stdlib.h>
#include <stdio.h> 
#include <fcntl.h> 
#include <sys/uio.h>
#include <string.h>
#define     NUM             2
#define     MAXBUFFSIZE     1024

struct my_buff {
    char        *buff;
    int         len;
};

void main(int argc, char *argv[]) {
    struct  my_buff     my_buff[NUM];
    struct iovec        iov[NUM];
    size_t      size;
    int         i,ret;
    int         fd;

    fd = open(argv[1],O_RDWR);
    if (fd < 0) {
        fprintf(stderr,"open error!\n");
        return;
    }

    for (i = 0; i < NUM; i++) {
        my_buff[i].buff = malloc(MAXBUFFSIZE);
        if (!my_buff[i].buff) {
            fprintf(stderr,"malloc error!\n");
            return;
        }

        strncpy(my_buff[i].buff,"three person!",111);//strlen("three person!"));
        my_buff[i].buff[strlen("three person!")+1] = '\0';

        iov[i].iov_base = my_buff[i].buff;
        iov[i].iov_len = strlen(my_buff[i].buff);
    }

    my_buff[1].buff[12] = '\0';
    //size = writev(fd,iov,2);
    size = write(fd,iov[1].iov_base,strlen(iov[1].iov_base)+1);
    fprintf(stderr,"size is %d\n",size);

}
