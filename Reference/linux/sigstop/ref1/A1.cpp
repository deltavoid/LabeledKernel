/*父进程fork子进程10秒后，向子进程发 SIGSTOP信号，让其停止运行。 
  再过5秒后，发 SIGCONT 让子进程恢复执行。
  再过20秒后，杀掉子进程，父进程也结束。
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_process()
{
    int i = 0;
    printf("Child start...\n");
    while (1)
    {
        sleep(1);
        printf("Child print %d\n", ++i);
    }
}
int main()
{
    int pid;
    printf("Fork child process ...\n");
    sleep(1);
    pid = fork();
    if (pid > 0)
    {
        printf("This is parent process, I will stop child process ~10 seconds later...\n");
        sleep(10);

        printf("Stop child process\n");
        kill(pid, SIGSTOP);
        
        printf("I will wake up child process 5 seconds later...\n");
        sleep(5);
        
        printf("Wake up child process\n");
        kill(pid, SIGCONT);
        
        /* after 20 seconds, kill child */
        sleep(20);
        kill(pid, SIGKILL);
        wait(NULL);
        printf("Bye\n");
    }
    else
    {
        child_process();
    }
    return 0;
}