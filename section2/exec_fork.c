#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <wait.h>

int main(void)
{
    int i, pid;
    char **argv = (char**)malloc(sizeof(char*) * 4);
    argv[0] = "/bin/ls";
    argv[1] = ".";
    argv[2] = "-al";
    argv[3] = NULL;
    
    for (i = 0; i < 10; i++)
    {
        printf("%d\n", i);
        if (i == 3)
        {
            pid = fork();
            if (pid == 0)
                execv("/bin/ls", argv);
            else
                wait(NULL);
        }
            
    }
    
    return 0;
}
