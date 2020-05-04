/* 58-timer.c --- timer */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static void
now()
{
    time_t t = time(0);
    printf("%s", ctime(&t));
}

static void
handler(int sig __attribute__((unused)))
{
    // psignal(sig, NULL);
    now();
}

int
main()
{
    if (signal(SIGALRM, handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    struct itimerval itv;
    itv.it_value.tv_sec = 3;
    itv.it_value.tv_usec = 0;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
        perror("setitimer");
        exit(EXIT_FAILURE);
    }

    now();
    printf("倒计时 %ld 秒...\n", itv.it_value.tv_sec);
    for (;;) {
        pause();
    }
}
