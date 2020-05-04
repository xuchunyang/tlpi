/* 59-read-timeout.c --- read with a timeout */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
handler(int sig)
{
    psignal(sig, 0);
}

int
main()
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, 0) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    alarm(5);

    char buf[20];
    ssize_t numRead = read(STDIN_FILENO, buf, sizeof buf);

    int savedErrno = errno;
    alarm(0);                   /* 清空 alarm */
    errno = savedErrno;

    if (numRead == -1) {
        if (errno == EINTR) {
            printf("timeout\n");
        } else {
            perror("read");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Read %ld bytes successed\n", numRead);
    }
}
