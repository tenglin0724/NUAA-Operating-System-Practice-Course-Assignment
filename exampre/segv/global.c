#include <stdio.h>

int array[10];

void func()
{
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
