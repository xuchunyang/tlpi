/* 61-nanosleep.c --- nanosleep */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <time.h>

static void
handler(int sig)
{
    psignal(sig, 0);
}

int
main()
{
    /* C-z */
    if (signal(SIGTSTP, handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    struct timespec request = {
        .tv_sec = 5,
        .tv_nsec = 0,
    };
    struct timespec remain = { 0 };
    if (nanosleep(&request, &remain) == 0) {
        printf("nanosleep successed\n");
    } else {
        perror("nanosleep");
        printf("remaing: %2ld.%09ld seconds\n",
               remain.tv_sec, remain.tv_nsec);
    }
}
