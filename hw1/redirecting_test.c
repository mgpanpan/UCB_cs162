#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define BUFLEN 256

extern char **environ;

int main(void)
{
    char buf[BUFLEN];
    char *argv[6];
    int i;
    
    for (i = 0; i < 6; i++)
        argv[i] = NULL;

    argv[0] = "/bin/ls";
    argv[1] = "-al";
    argv[2] = ">";
    argv[3] = "out.txt";
        
    fgets(buf, BUFLEN, stdin);

    fputs(buf, stdout);

    for (i = 0; environ[i]; i++)
        fprintf(stdout, "%s\n", environ[i]);
    
    /* execve does not support redirecting */
    /* if (fork() == 0) */
    /* { */
    /*     execve(argv[0], argv, environ); */
    /*     exit(0); */
    /* } */
    /* else */
    /*     waitpid(-1, NULL, 0); */
            
    return 0;
}
