#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


const int N = 4;
pthread_t pthreads[N];


void* entry(void* arg)
{
    int a = 1, b = 1, c = 1;
    while (true)
    {
        c = a + b;
    }

    return NULL;
}


int main(int argc, char** argv)
{
    int T = 10;
    if  (argc > 1)  sscanf(argv[1], "%d", &T);

    for (int i = 0; i < N; i++)
    {
        pthread_create(&pthreads[i], NULL, entry, NULL);
    }

    sleep(T);

    return 0;
}