/* 76-orphan.c --- orphan child process */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void
handler(int sig)
{
    psignal(sig, 0);
}

int
main()
{
    if (signal(SIGUSR1, handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    pid_t childPid = fork();
    switch (childPid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0:
        printf("Child id process = %d, parent process id = %ld\n", getpid(), (long) getppid());
        sleep(2);
        printf("Child id process = %d, parent process id = %ld\n", getpid(), (long) getppid());
        _exit(EXIT_SUCCESS);

    default:
        sleep(1);
        exit(EXIT_SUCCESS);
    }
}
