/* 104-simple-pipe.c --- simple pipe(2) use */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int
main()
{
    int fildes[2];
    if (pipe(fildes) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t childPid = fork();
    if (childPid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (childPid == 0) {
        if (close(fildes[1]) == -1) {
            perror("close - child");
            exit(EXIT_FAILURE);
        }

        char buf[1024] = {0};
        ssize_t numRead;
        while(1) {
            numRead = read(fildes[0], buf, sizeof buf);
            if (numRead == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            if (numRead == 0)
                break;

            if (write(STDOUT_FILENO, buf, numRead) == -1) {
                perror("write - child");
                exit(EXIT_FAILURE);
            }
        }
        _exit(EXIT_SUCCESS);
    } else {
        if (close(fildes[0]) == -1) {
            perror("close - parent");
            exit(EXIT_FAILURE);
        }

        char msg[] = "hello, world!";
        int len = sizeof msg - 1;
        if (write(fildes[1], msg, len) != len) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        if (close(fildes[1]) == -1) {
            perror("close - parent");
            exit(EXIT_FAILURE);
        }

        wait(NULL);
    }
}
