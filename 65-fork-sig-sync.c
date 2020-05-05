/* 65-fork-sig-sync.c --- 用信号同步进程 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

static char*
now()
{
    time_t t = time(0);
    char* s = ctime(&t);
    return strndup(s, strlen(s)-1);
}

static void
handler(int sig __attribute__((unused)))
{

}

int
main()
{
    setbuf(stdout, 0);

    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &blockMask, 0) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SIGUSR1, &sa, 0) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pid_t childPid;
    switch (childPid = fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0:
        printf("[%s %ld] Child started - doing some work\n", now(), (long) getpid());
        sleep(2);

        if (kill(getppid(), SIGUSR1) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }

        printf("[%s %ld] Child about to quit\n", now(), (long) getpid());
        _exit(EXIT_SUCCESS);

    default:
        printf("[%s %ld] Parent about to wait for signal\n", now(), (long) getpid());

        sigset_t emptyMask;
        sigemptyset(&emptyMask);
        if (sigsuspend(&emptyMask) == -1 && errno != EINTR) {
            perror("sigsuspend");
            exit(EXIT_FAILURE);
        }

        printf("[%s %ld] Parent got signal\n", now(), (long) getpid());

        exit(EXIT_SUCCESS);
    }
}
