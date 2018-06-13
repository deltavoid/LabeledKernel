#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
//#include <sys/wait.h>



int main(int argc, char** argv)
{
    assert(argc > 1);
    int pid;
    sscanf(argv[1], "%d", &pid);

    int op;
    while (scanf("%d", &op) != EOF && op != 0)
    {
        if  (op == 1)  kill(pid, SIGCONT);
        else if  (op == 2)  kill(pid, SIGSTOP);

    }

    kill(pid, SIGKILL);
    return 0;
}