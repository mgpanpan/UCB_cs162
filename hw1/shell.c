#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FALSE 0
#define TRUE 1
#define INPUT_STRING_SIZE 80

#define BUFLEN 256

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"


static int is_bg = 0;    /* whether is background process */

int cmd_quit(tok_t arg[])
{
    printf("Bye\n");
    exit(0);
    return 1;
}

int cmd_help(tok_t arg[]);
int cmd_cd(tok_t arg[]);

/* Command Lookup table */
typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc
{
    cmd_fun_t *fun;
    char *cmd;
    char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] =
{
    {cmd_help, "?", "show this help menu"},
    {cmd_quit, "quit", "quit the command shell"},
    {cmd_cd, "cd", "change directory"}
};

int cmd_help(tok_t arg[])
{
    int i;
    for (i = 0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++)
        printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);

    return 1;
}

int cmd_cd(tok_t arg[])
{
    if (chdir(arg[0]) < 0)
    {
        fprintf(stderr, "chdir error\n");
        return -1;
    }
    return 1;
}

int lookup(char cmd[])
{
    int i;
    for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++)
        if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;

    return -1;
}

void init_shell()
{
    /* Check if we are running interactively */
    shell_terminal = STDIN_FILENO;

    /** Note that we cannot take control of the terminal if the shell
        is not interactive */
    shell_is_interactive = isatty(shell_terminal);

    if (shell_is_interactive)
    {
        /* force into foreground */
        while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
            kill( - shell_pgid, SIGTTIN);
        
        shell_pgid = getpid();
        /* Put shell in its own process group */
        if(setpgid(shell_pgid, shell_pgid) < 0)
        {
            perror("Couldn't put the shell in its own process group");
            exit(1);
        }
        
        /* Take control of the terminal */
        tcsetpgrp(shell_terminal, shell_pgid);
        tcgetattr(shell_terminal, &shell_tmodes);
    }

    /** YOUR CODE HERE */
    /* initialize the process list */
//    first_process = 
}

/**
 * Add a process to our process list
 */
void add_process(process* p)
{
  /** YOUR CODE HERE */
}

/**
 * Creates a process given the inputString from stdin
 */
process* create_process(char* inputString)
{
    /** YOUR CODE HERE */
    
    return NULL;
}


extern char **environ;
int shell (int argc, char *argv[])
{
    char *s = malloc(INPUT_STRING_SIZE+1);			/* user input string */
    char *s_copy = malloc(INPUT_STRING_SIZE+1);
    
    tok_t *t;			/* tokens parsed from input */
    int lineNum = 0;
    int fundex = -1;
    pid_t pid = getpid();		/* get current processes PID */
    pid_t ppid = getppid();	/* get parents PID */
    pid_t cpid, tcpid, cpgid;
    char *path = getenv("PATH");
    char *path_iter;
    char full_path[BUFLEN];
    
    char buf[BUFLEN];
    int in_redirect_flag = 0;
    int out_redirect_flag = 0;

    int i, j;
    FILE *infile;
    FILE *outfile;
    int stdin_bak;
    int stdout_bak;

    int argc;

    init_shell();

    printf("%s running as PID %d under %d\n", argv[0], pid, ppid);

    lineNum = 0;
    
    getcwd(buf, BUFLEN);
    fprintf(stdout, "%d: %s: ", lineNum++, buf);
    
    while ((s = freadln(stdin)))
    {
        is_bg = 0;
        strncpy(s_copy, s, INPUT_STRING_SIZE);
        
        t = getToks(s); /* break the line into tokens */

        for (i = 0; t[i]; i++)
            if (!strcmp(t[i], "<"))
            {
                if (!(infile = fopen(t[i+1], "r")))
                {
                    fprintf(stderr, "fopen error\n");
                    break;
                }
                stdin_bak = dup(STDIN_FILENO);
                if (dup2(fileno(infile), STDIN_FILENO) < 0)
                {
                    fprintf(stderr, "dup2 error\n");
                    break;
                }
                fclose(infile);
                t[i] = NULL; t[i+1] = NULL;
                for (j = i+2; t[j]; j++)    /* delete the "< in.txt" */
                    t[j-2] = t[j];

                in_redirect_flag = 1;
                break;
            }
        
        for (i = 0; t[i]; i++)
            if (!strcmp(t[i], ">"))
            {
                if (!(outfile = fopen(t[i+1], "w+")))
                {
                    fprintf(stderr, "fopen error\n");
                    break;
                }
                stdout_bak = dup(STDOUT_FILENO);
                if (dup2(fileno(outfile), STDOUT_FILENO) < 0)
                {
                    fprintf(stderr, "dup2 error\n");
                    break;
                }
                fclose(outfile);
                t[i] = NULL; t[i+1] = NULL;
                for (j = i+2; t[j]; j++)    /* delete the "> out.txt" */
                    t[j-2] = t[j];
                out_redirect_flag = 1;
                
                break;
            }

        for (i = 0; t[i]; i++)
            argc++;

        if (argc > 0 && !strcmp(t[argc-1], "&"))
        {
            t[--argc] = NULL;
            is_bg = 1;
        }
            
        fundex = lookup(t[0]); /* Is first token a shell literal */
        
        if (fundex >= 0) cmd_table[fundex].fun(&t[1]);
        else
        {
            cpid = fork();
            if (cpid == 0)    /* child process */
            {
                if (execve(t[0], (char **)t, environ) < 0)
                {
                    path_iter = strtok(path, ":");
                    strncpy(full_path, path_iter, BUFLEN);
                    while (path_iter &&
                           (execve(strcat(strcat(full_path, "/"), t[0]), (char **)t, environ) < 0))
                    {
                        path_iter = strtok(NULL, ":");
                        if (path_iter) strncpy(full_path, path_iter, BUFLEN);
                    }
                    fprintf(stderr, "execve error\n");
                    exit(0);
                }
            }
            else if (cpid > 0) /* parent process */
            {
                if (!bg)    /* foreground job => wait until terminate */
                    if (waitpid(-1, NULL, 0) < 0)
                        fprintf(stderr, "waitpid error\n");
            }
            else
                fprintf(stderr, "fork error\n");
        }       

        if (in_redirect_flag)
        {
            dup2(stdin_bak, STDIN_FILENO);
            close(stdin_bak);
        }
        if (out_redirect_flag)
        {
            dup2(stdout_bak, STDOUT_FILENO);
            close(stdout_bak);
        }
        
        in_redirect_flag = 0;
        out_redirect_flag = 0;
        getcwd(buf, BUFLEN);
        fprintf(stdout, "%d: %s: ", lineNum++, buf);
    }
    return 0;
}
