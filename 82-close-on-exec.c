/* 82-close-on-exec.c --- close file on exec */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>
int
main(int argc, char* argv[])
{
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        printf("usage: %s [n]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc > 1) {             /* enable close-on-exec */
        printf("Turn on close-on-exec on stdout\n");
        int flags = fcntl(STDOUT_FILENO, F_GETFD);
        if (flags == -1) {
            perror("fcntl");
            exit(EXIT_FAILURE);
        }

        flags |= FD_CLOEXEC;
        if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1) {
            perror("fcntl");
            exit(EXIT_FAILURE);
        }
    }

    printf("running ls -l\n");
    execlp("ls", "ls", "-l", NULL);
    perror("execlp");
    exit(EXIT_FAILURE);
}
