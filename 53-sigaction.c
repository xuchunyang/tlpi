/* 53-sigaction.c --- 处理 signal */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
handler(int sig)
{
    psignal(sig, NULL);
    printf("sleep 3s in handler...\n");
    sleep(3);
    printf("sleep 3s in handler...done\n");
}

int
main()
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    for(;;)
        sleep(3);
}
