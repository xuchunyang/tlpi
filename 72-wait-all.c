/* 72-wait-all.c --- wait for all child processes to quit */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        int sec = strtol(argv[i], 0, 10);
        if (errno) {
            perror("strtol");
            exit(EXIT_FAILURE);
        }

        switch (fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);

        case 0:
            printf("child id=%ld sleep for %ds\n", (long)getpid(), sec);
            sleep(sec);
            _exit(EXIT_FAILURE);
        }
    }

    printf("should wait for child process to quit\n");

    pid_t childPid;
    for(;;) {
        childPid = wait(0);
        if (childPid == -1) {
            if (errno == ECHILD) {
                printf("no more child process\n");
                exit(EXIT_SUCCESS);
            } else {
                perror("wait");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("child id=%ld quited\n", (long)childPid);
        }
    }

}
