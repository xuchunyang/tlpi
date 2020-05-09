/* 100-SIGTTIN.c --- (libc) Access to the Terminal */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
handler(int sig)
{
    psignal(sig, NULL);
}

int
main()
{
    if (signal(SIGTTIN, handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    ssize_t numRead = read(STDIN_FILENO, buf, sizeof buf);
    if (numRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    } else {
        printf("Read %zd bytes\n", numRead);
    }
}
