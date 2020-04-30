/* 24-argv.c --- echo argv */

#include <stdio.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
        printf("argv[%d]: %s\n", i, argv[i]);

    for (char** p = argv; *p; p++)
        puts(*p);

    printf("ARG_MAX: %ld\n", sysconf(_SC_ARG_MAX));
}
