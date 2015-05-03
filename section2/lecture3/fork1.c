#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    pid_t cpid, mypid;
    pid_t pid = getpid();    /* get current processes PID */
    printf("Parent pid: %d\n", pid);
    cpid = fork();
    if (cpid > 0)    /* parent process */
    {
        mypid = getpid();
        printf("[%d] parent of [%d]\n", mypid, cpid);
    }
    else if (cpid == 0)  /* child process */
    {
        mypid = getpid();
        printf("[%d] child\n", mypid);
    }
    else
    {
        perror("Fork failed");
        exit(1);
    }

    exit(0);
}
