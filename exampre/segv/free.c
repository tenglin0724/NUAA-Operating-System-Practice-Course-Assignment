#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void func()
{
    char *p = malloc(10);
    strcpy(p, "hello");
    free(p);
    strcpy(p, "world");
}

int main()
{
    func();
    puts("----------END------------");
    return 0;
}
