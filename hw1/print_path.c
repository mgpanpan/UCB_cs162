#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    char *path = getenv("PATH");
    char *path_iter;
    printf("%s\n", path);

    path_iter = strtok(path, ":");
    while (path_iter)
    {
        printf("%s\n", path_iter);
        path_iter = strtok(NULL, ":");
    }
    
    return 0;
}
