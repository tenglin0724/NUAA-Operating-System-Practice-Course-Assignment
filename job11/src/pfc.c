#include <stdio.h>
#include <stdlib.h>
#include "lib/coro.h"

coro_t *coro_p;
coro_t *coro_c;
coro_t *coro_f;

void produce()
{
    puts("PRODUCE");

    for(int i=0;i<8;i++){
        printf("produce %d\n",i);
        coro_yield(i);
    }

    puts("NEVER REACH HERE");
}

void filter(){
    int x;
    puts("FILTER");

    for(int i=0;i<8;i++){
        x=coro_resume(coro_p);
        printf("filter %d->%d\n",x,x*10);
        coro_yield(i);
    }

}

void consume()
{
    int x;

    puts("\tCONSUME");

    for(int i=0;i<8;i++){
        x = coro_resume(coro_f);
        printf("\tconsume %d\n", x);
    }

    puts("END");
    exit(0);
}

int main()
{
    coro_p = coro_new(produce);
    coro_c = coro_new(consume);
    coro_f = coro_new(filter);
    coro_boot(coro_c);
    return 0;
}
