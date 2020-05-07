/* 84-system.c --- impl system */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void
handler(int sig)
{
    psignal(sig, NULL);
}

int
mysystem(char* cmd)
{
    int status;
    pid_t childPid = fork();
    switch (childPid) {
    case -1:
        return -1;
    case 0:
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        _exit(127);
    default:
        if (waitpid(childPid, &status, 0) == -1)
            return -1;
        else
            return status;
    }
}

int
main()
{
    signal(SIGINT, handler);

    mysystem("sleep 10");       /* FIXME Not Finished */
    printf("Over!\n");

    sleep(3);
}
