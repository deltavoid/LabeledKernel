#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    for (int i = 1; ; i++)
    {
        printf("child print %d\n", i);
        sleep(1);
    }
    return 0;
}