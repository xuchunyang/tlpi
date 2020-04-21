/* 02-perror.c --- perror */
#include <stdio.h>
#include <errno.h>

int
main(void)
{
    for (int i = 1; i < 150; ++i) {
        fprintf(stderr, "errno %d: ", i);
        errno = i;
        perror(0);
    }
}
