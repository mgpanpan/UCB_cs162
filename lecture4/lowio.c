#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFSIZE 32

int main(void)
{
    char buf[BUFSIZE];
    ssize_t writelen = write(STDOUT_FILENO, "I am a process.\n", 16);

    ssize_t readlen = read(STDIN_FILENO, buf, BUFSIZE);

    ssize_t status;
    writelen = readlen < BUFSIZE ? readlen : BUFSIZE;
    
    status = write(STDOUT_FILENO, buf, writelen);

    printf("\n%zd\n", status);
    
    ssize_t strlen = snprintf(buf, BUFSIZE, "Got %zd chars\n", readlen);

    writelen = strlen < BUFSIZE ? strlen : BUFSIZE;
    status = write(STDOUT_FILENO, buf, writelen);

    printf("\n%zd\n", status);
    
    exit(0);
}
