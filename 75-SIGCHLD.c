/* 75-SIGCHLD.c --- SIGHILD */
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
    if (signal(SIGCHLD, handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

#define NP 3

    for (int i = 0; i < NP; i++)
        switch (fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            _exit(EXIT_FAILURE);
        }

    for(int i = 0; i < NP; i++)
        pause();
}
