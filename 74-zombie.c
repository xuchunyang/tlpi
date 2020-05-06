/* 74-zombie.c --- 僵尸进程 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <libgen.h>             /* libgen */

char buf[1024];

int
main(int argc __attribute__((unused)),
     char* argv[])
{
    pid_t childPid = fork();
    switch(childPid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0:
        printf("child process is running\n");
        sleep(1);
        printf("child process about to quit\n");
        _exit(EXIT_FAILURE);

    default:
        snprintf(buf, sizeof buf, "ps | grep %s",
                 basename(argv[0]));
        system(buf);
        if (kill(childPid, SIGKILL) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }
        system(buf);
    }
}
