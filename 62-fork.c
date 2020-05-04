/* 62-fork.c --- fork */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main()
{
    switch (fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;

    case 0:
        printf("child process\n");
        break;

    default:
        sleep(3);
        printf("parent process\n");
        break;
    }
}
