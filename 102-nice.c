/* 102-nice.c --- getpriority(2) */
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <errno.h>

int
main()
{
    if (setpriority(PRIO_PROCESS, 0, 10) == -1) {
        perror("setpriority");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    int prio = getpriority(PRIO_PROCESS, 0);
    if (errno != 0) {
        perror("getpriority");
        exit(EXIT_FAILURE);
    }
    printf("getpriority: %d\n", prio);
}
