/* 46-realpath.c --- ex18-3 实现 realpath */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>

char *
my_realpath(const char *restrict file_name __attribute__((unused)),
            char *restrict resolved_name   __attribute__((unused)))
{
    return 0;
}

int
main(int argc, char* argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s file...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* file = argv[1];
    char* initial = file;
    char buf[PATH_MAX];
    for(;;) {
        struct stat statbuf;
        if (lstat(file, &statbuf) == -1) {
            printf("lstat %s: %s\n", file, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (S_ISLNK(statbuf.st_mode)) {
            printf("file: %s\n", file);
            int fd = open(file, O_RDONLY);
            ssize_t numBytes =
                // readlink(file, buf, sizeof buf);
                /* TODO incorrect */
                readlinkat(fd, file, buf, sizeof buf);
            if (numBytes == -1) {
                perror("readlink");
                exit(EXIT_FAILURE);
            }
            buf[numBytes] = 0;
            file = buf;
        } else {
            printf("%s -> %s\n", initial, file);
            break;
        }
    }
}
