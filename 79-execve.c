/* 79-execve.c --- execve */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("usage: %s program\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *const argVec[] = {"app-basename", "hello", "goodbye", NULL};
    char *const envVec[] = {NULL};
    execve(argv[1], argVec, envVec);
    perror("execve");
    exit(EXIT_FAILURE);
}
