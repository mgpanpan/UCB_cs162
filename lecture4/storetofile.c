#include <stdio.h>
#include <string.h>

#define BUFLEN 256

int main(void)
{
    FILE *outfile = fopen("./outfile.txt", "w+");
    char buf[BUFLEN];
    char *inputline;
    
    if (!outfile)
        return -1;

    while (1)
    {
        inputline = fgets(buf, BUFLEN, stdin);

        if (!inputline || strlen(inputline) == 0) break;

        if (fputs(inputline, outfile) < 0) break;
    }
    fclose(outfile);

    return 0;
}
