/* 80-exec.c --- exec() familiy */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <unistd.h>

int
main()
{
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "--version", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *argv[] = {"git", "--version", NULL};
        execv("/usr/bin/git", argv);
        perror("execv");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execlp("cc", "cc", "--version", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *argv[] = {"emacs", "emacs", "--version", NULL};
        execvp("emacs", argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *env[] = {"LC_TIME=zh_CN.UTF-8", NULL};
        execle("/bin/sh", "sh", "-c", "date", NULL, env);
        perror("execle");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *argv[] = {"grep", "--version", NULL};
        char *env[] = {NULL};
        execve("/usr/bin/grep", argv, env);
        perror("execve");
        exit(EXIT_FAILURE);
    }

    for(;;) {
        pid_t childPid = wait(NULL);
        if (childPid == -1) {
            if (errno == ECHILD) {
                printf("No more child processes\n");
                exit(EXIT_SUCCESS);
            } else {
                perror("wait");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("child process %ld quited\n",
                   (long) childPid);
        }
    }

}
