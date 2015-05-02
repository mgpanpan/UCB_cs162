#include <stdio.h>
#include <ctype.h>

#define IN 1
#define OUT 0

void wc(FILE *ofile, FILE *infile, char *inname)
{
    int nl = 0, nw = 0, nc = 0;
    int state = OUT;
    int c;
    /* nl : line numbers, nw : word numbers, nc : character numbers */
    while ((c = fgetc(infile)) != EOF)
    {
        nc++;
        if (c == '\n') nl++;
        if (c == '\t' || c == '\n' || c == ' ')
            state = OUT;
        else if (state == OUT)
        {
            state = IN;
            nw++;
        }
    }
    fprintf(ofile, "%d\t%d\t%d\t%s\n", nl, nw, nc, inname);
}

int main (int argc, char *argv[])
{
    FILE *infile, *ofile;
    
    if (argc == 1)
    {
        infile = stdin;
        ofile = stdout;
        wc(ofile, infile, "standard input");
    }
    else if (argc == 2)
    {
        infile = fopen(argv[1], "r");
        ofile = stdout;
        wc(ofile, infile, argv[1]);
    }
    else if (argc == 3)
    {
        infile = fopen(argv[1], "r");
        ofile = fopen(argv[2], "w");
        wc(ofile, infile, argv[1]);
    }

    return 0;
}
