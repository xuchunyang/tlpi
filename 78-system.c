/* 78-system.c -- system(3) */
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main()
{
    if (system(NULL) == 0) {
        printf("No shell is available\n");
        exit(EXIT_FAILURE);
    }

    /* Does system(3) use /bin/sh? */
    system("echo $SHELL $0");
    system("$0 --version");

    char cmd[1024];
    /* INT and QUIT are ignored */
    snprintf(cmd, sizeof cmd, "kill -s INT %d", getpid());
    printf("about to run '%s'\n", cmd);
    int status = system(cmd);
    if (status == -1) {
        perror("system");
        exit(EXIT_FAILURE);
    } else {
        if (WIFEXITED(status))
            printf("system() exited with %d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("system() terminated with signal %d\n", WTERMSIG(status));
        else
            printf("should not reach here???\n");
    }

    sleep(1);
    printf("over\n");

    status = system("lsss");
    if (WIFEXITED(status)) {
        printf("=> exited with %d\n", WEXITSTATUS(status));
    }
}
