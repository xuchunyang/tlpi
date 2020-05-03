/* 51-signal.c --- handle signal */

#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
sigHander(int sig)
{
    printf("Received signal %d, %s\n", sig, strsignal(sig));
}

int
main()
{
    /* C-z */
    if (signal(SIGTSTP, sigHander) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    /* C-c */
    if (signal(SIGINT, sigHander) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    sig_t prevSighandler = signal(SIGUSR1, sigHander);
    if (prevSighandler == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    if (prevSighandler == SIG_DFL) {
        printf("prevSighandler is the default signal handler\n");
    }

    // getchar();

    for (int i = 0; ; i++) {
        printf("%d\n", i);
        sleep(3);
    }

}
