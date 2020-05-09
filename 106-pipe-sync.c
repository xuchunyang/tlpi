/* 106-pipe-sync.c --- sync processes with pipe */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void
errExit(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

static long
getLong(char *s)
{
    errno = 0;
    long res = strtol(s, NULL, 10);
    if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }
    return res;
}

int
main(int argc, char* argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s sleep-time...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fds[2];
    if (pipe(fds) == -1)
        errExit("pipe");

    for (int i = 1; i < argc; i++) {
        switch(fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);

        case 0:
            if (close(fds[0]) == -1)
                errExit("close");

            sleep(getLong(argv[i]));

            printf("Child %d (PID=%ld) closing pipe\n", i, (long) getpid());

            if (close(fds[1]) == -1)
                errExit("close");

            _exit(EXIT_SUCCESS);
        }
    }
    printf("Parent started\n");
    if (close(fds[1]) == -1)
        errExit("close");

    int dummy;
    if (read(fds[0], &dummy, 1) == -1)
        errExit("read");

    printf("Parent ready to go!\n");
}
