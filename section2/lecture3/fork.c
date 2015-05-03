#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    pid_t cpid, mypid;
    int i;
    cpid = fork();
    if (cpid > 0)
    {
        mypid = getpid();
        printf("[%d] parent of [%d]\n", mypid, cpid);
        for (i = 0; i < 100; i++)
        {
            printf("[%d] parent: %d\n", mypid, i);
            sleep(1);
        }
    }
    else if (cpid == 0)
    {
        mypid = getpid();
        printf("[%d] child\n", mypid);
        for (i = 0; i > -100; i--)
        {
            printf("[%d] child: %d\n", mypid, i);
            // sleep(1);
        }
    }
    else
    {
        perror("Fork failed");
        exit(1);
    }

    exit(0);
}
