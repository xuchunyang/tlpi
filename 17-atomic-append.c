/* 17-atomic-append.c --- ex5-3 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>

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
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s file num-bytes [x]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    bool x = false;
    if (argc == 4 && strcmp(argv[3], "x") == 0) {
        x = true;
        printf("race cond\n");
    }

    int fd;
    if (x)
        fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    else
        fd = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    long numBytes = getLong(argv[2]);
    for (int i = 0; i < numBytes; i++) {
        if (x)
            assert(lseek(fd, 0, SEEK_END) != -1);
        assert(write(fd, "x", 1) == 1);
    }

    assert(close(fd) == 0);
}
