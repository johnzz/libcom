#include <stdio.h>


void change(void *i){
    int *temp = i;
    printf("before change %d\n",*temp);
}

void main()
{
    int     i = 1,j;

    for (j = 0; j < 5; j++) {
        change(&j);
    }

}
