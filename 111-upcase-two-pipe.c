/* 111-upcase-two-pipe.c --- ex44-1 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
errExit(char* s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

int
main()
{
    int fds1[2];
    if (pipe(fds1) == -1)
        errExit("pipe");

    int fds2[2];
    if (pipe(fds2) == -1)
        errExit("pipe");

    int pid = fork();
    if (pid == -1)
        errExit("fork");

    if (pid == 0) {             /* child */
        if (close(fds1[1]) == -1)
            errExit("close pipe1 write");

        if (close(fds2[0]) == -1)
            errExit("close pipe2 read");


        while(1) {
            char buf[1024];
            ssize_t numRead = read(fds1[0], buf, sizeof buf);
            if (numRead == -1)
                errExit("read");
            if (numRead == 0)       /* EOF */
                break;
            for (int i = 0; i < numRead; i++) {
                buf[i] = toupper(buf[i]);
            }
            if (write(fds2[1], buf, numRead) != numRead)
                errExit("write to pipe1");
        }

        if (close(fds1[0]) == -1)
            errExit("close pipe1 read");
        if (close(fds2[1]) == -1)
            errExit("close pipe2 write");

        _exit(EXIT_SUCCESS);
    }

    if (close(fds1[0]) == -1)
        errExit("parent close pipe1 read");

    /* parent */
    while(1) {
        char buf[1024];
        ssize_t numRead = read(STDIN_FILENO, buf, sizeof buf);
        if (numRead == -1)
            errExit("read");
        if (numRead == 0)       /* EOF */
            break;
        if (write(fds1[1], buf, numRead) != numRead)
            errExit("write to pipe1");
    }

    if (close(fds1[1]) == -1)
        errExit("close pipe1 write");

    if (close(fds2[1]) == -1)
        errExit("close pipe2 write");

    while(1) {
        char buf[1024];
        ssize_t numRead = read(fds2[0], buf, sizeof buf);
        if (numRead == -1)
            errExit("read");
        if (numRead == 0)       /* EOF */
            break;
        if (write(STDOUT_FILENO, buf, numRead) != numRead)
            errExit("write to stdout");
    }

    if (close(fds2[0]) == -1)
        errExit("close pipe2 read");
}
