/* 16-append.c --- ex5-2. 测试 O_APPEND */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // O_APPEND        append on each write
    int fd = open(argv[1], O_WRONLY | O_EXCL | O_APPEND);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(fd, 0, SEEK_SET);
    if (offset == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }
    if (offset != 0) {
        printf("offset: want 0, get %lld\n", offset);
        exit(EXIT_FAILURE);
    }

    char* s = "hello";
    ssize_t numWritten = write(fd, s, strlen(s));
    if (numWritten == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    ssize_t totRequired = strlen(s);
    if (numWritten < totRequired)
        printf("numWritten: %zd, total: %zd\n", numWritten, totRequired);

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
