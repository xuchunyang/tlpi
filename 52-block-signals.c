/* 52-block-signals.c --- block signal with sigprocmask */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void
sigHander(int sig)
{
    psignal(sig, 0);
}

int
main()
{
    if (signal(SIGINT, sigHander) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    sigset_t blockSet;
    sigset_t preMask;

    sigemptyset(&blockSet);
    sigaddset(&blockSet, SIGINT); /* C-c */

    if (sigprocmask(SIG_BLOCK, &blockSet, &preMask) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    printf("SIGINT blocked\n");
    for(;;)
        if (getchar() == 'q')
            break;

    if (sigprocmask(SIG_SETMASK, &preMask, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    printf("SIGINT unblocked\n");
    for(;;)
        getchar();
}
