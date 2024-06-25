#include <stdio.h>
#include <stdlib.h>

void func()
{
    int *array = malloc(10 * sizeof(int));
    int i;
    for (i = 1; i <= 10; i++)
        array[i] = i;
}

int main()
{
    func();
    puts("----------END------------");
    return 0;
}
