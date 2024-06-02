#include <stdio.h>
#include <stdlib.h>
#include "lib/coro.h"

coro_t *coro_p;
coro_t *coro_c;

void produce()
{
    puts("PRODUCE");

    for(int i=0;i<8;i++){
        printf("produce %d\n",i);
        coro_yield(i);
    }

    puts("NEVER REACH HERE");
}

void consume()
{
    int x;

    puts("\tCONSUME");

    for(int i=0;i<8;i++){
        x = coro_resume(coro_p);
        printf("\tconsume %d\n", x);
    }

    puts("END");
    exit(0);
}

int main()
{
    coro_p = coro_new(produce);
    coro_c = coro_new(consume);
    coro_boot(coro_c);
    return 0;
}
