/* 79-execve.c --- execve */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main()
{
    char *const argVec[] = {"git", "--version", NULL};
    char *const envVec[] = {NULL};
    execve("/usr/bin/git", argVec, envVec);
    perror("execve");
    exit(EXIT_FAILURE);
}
