/* 54-pause.c --- 等待信号 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

static void
handler(int sig)
{
    psignal(sig, 0);
}

int
main()
{
    if (signal(SIGINT, handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    assert(pause() == -1);
    assert(errno == EINTR);

    for(;;) {
        printf("waiting for a signal...\n");
        pause();
        printf("waiting for a signal...done\n");
    }
}
