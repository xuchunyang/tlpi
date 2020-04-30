/* 11-pwrite.c --- pwrite */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/stat.h> // S_IRUSR

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

int
main(int argc, char* argv[])
{
    if (argc != 4 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s file offset string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    off_t offset = getLong(argv[2]);
    printf("offset: %lld\n", (long long) offset);
    ssize_t numWritten = pwrite(fd, argv[3], strlen(argv[3]), offset);
    if (numWritten == -1) {
        perror("pwrite");
        exit(EXIT_FAILURE);
    }
    printf("wrote %ld bytes\n", numWritten);

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
