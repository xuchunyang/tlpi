/* 14-truncate.c -- truncate */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static long
getLong(const char *s)
{
    errno = 0;
    long num = strtol(s, 0, 10);
    if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }
    return num;
}

int main(int argc, char* argv[])
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s file size\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    off_t offset = getLong(argv[2]);
    if (truncate(argv[1], offset) == -1) {
        perror("truncate");
        exit(EXIT_FAILURE);
    }
}
