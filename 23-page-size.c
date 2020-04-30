/* 23-page-size.c --- page size */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main()
{
    long size = sysconf(_SC_PAGESIZE);
    if (size == -1) {
        perror("sysconf");
        exit(EXIT_FAILURE);
    }
    printf("Page size: %ld bytes\n", size);
}
