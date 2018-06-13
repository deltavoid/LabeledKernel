#include <cstdio>
#include <cstdlib>
#include <unistd.h>


int main()
{
    while (true)
    {
        int length = 1 << 24;
        int* p = (int*)malloc(length * sizeof(int));
        for (int i = 0; i < length; i++)
            p[i] = 0;
        sleep(1);
    }
    return 0;
}