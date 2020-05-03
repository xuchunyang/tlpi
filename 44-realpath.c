/* 44-realpath.c --- realpath */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char buf[PATH_MAX];
    if (realpath(argv[1], buf) == NULL) {
        perror("realpath");
        exit(EXIT_FAILURE);
    }
    printf("realpath: %s -> %s\n", argv[1], buf);

    struct stat statbuf;
    if (lstat(argv[1], &statbuf) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    if (S_ISLNK(statbuf.st_mode)) {
        ssize_t numBytes = readlink(argv[1], buf, sizeof buf);
        if (numBytes == -1) {
            perror("readlink");
            exit(EXIT_FAILURE);
        }
        buf[numBytes] = 0;
        printf("readlink: %s -> %s\n", argv[1], buf);
    }

    snprintf(buf, sizeof buf, "ls -ld %s", argv[1]);
    system(buf);
}
