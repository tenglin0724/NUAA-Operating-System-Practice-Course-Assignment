#include <stdio.h>

void func()
{
    int array[10];
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
