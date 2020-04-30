/* 10-pread.c --- pread */
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define BUF_SIZE 7

int
main()
{
    int fd = open("10-pread.c", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    off_t offset = 28;
    ssize_t numRead = pread(fd, buf, BUF_SIZE, offset);
    if (numRead == -1) {
        perror("pread");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, buf, numRead);
    printf("\n");

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
