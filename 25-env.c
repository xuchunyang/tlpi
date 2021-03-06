/* 25-env.c --- 环境变量 */

#include <stdio.h>
#include <stdlib.h>

extern char** environ;

int main()
{
    /* Print env */
    for (char** p = environ; *p; p++)
        printf("%s\n", *p);

    char* home = getenv("HOME");
    if (home == NULL)
        printf("NO HOME\n");
    else
        printf("$HOME is %s\n", home);

    if (setenv("MSG", "hello", 0) != 0) {
        perror("setenv");
        exit(EXIT_FAILURE);
    }
    if (setenv("MSG", "HELLO", 0) != 0) {
        perror("setenv");
        exit(EXIT_FAILURE);

    }
    for (char** ep = environ; *ep; ep++)
        puts(*ep);
}
