/* 07-print-holes.c --- Print holes */
#define _GNU_SOURCE
#include <errno.h>
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

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("opening %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    off_t offset = 0;
    while (1) {
        offset = lseek(fd, offset, SEEK_HOLE);
        if (offset == -1) {
            if (errno != ENXIO)
                perror("lseek SEEK_HOLE");
            break;
        }
        printf("hole offset: %lld\n", (long long) offset);
        offset = lseek(fd, offset, SEEK_DATA);
        if (offset == -1) {
            if (errno != ENXIO)
                perror("lseek SEEK_DATA");
            break;
        }
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
