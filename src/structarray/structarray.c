#include "structarray.h"

int libcomm_structarray_handler(void *data)
{
    char    *name = (char *)data;
    printf("handler to handle %s\n",name);
    return 1;

}

libcomm_structarray_t   libcomm_structarray[] = {
    {"host",    offsetof(libcomm_structarray_member_t,host),
                                    libcomm_structarray_handler},

    {"agent",    offsetof(libcomm_structarray_member_t,agent),
                                    libcomm_structarray_handler},

    {"ip",    offsetof(libcomm_structarray_member_t,ip),
                                    libcomm_structarray_handler},


    {"port",    offsetof(libcomm_structarray_member_t,port),
                                    libcomm_structarray_handler},
};

void main(){

    libcomm_structarray_t   st;
    int         len,i;
    void        *data;

    len = sizeof(libcomm_structarray) / sizeof(libcomm_structarray_t);

    printf("len is %d\n",len);

    for (i = 0; i < len; i++) {
        st = libcomm_structarray[i];
        data = libcomm_structarray[i].name;
        st.handler(data);
    }

}
