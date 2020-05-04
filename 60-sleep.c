/* 60-sleep.c --- sleep */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void
handler(int sig)
{
    psignal(sig, 0);
}

int
main()
{
    /* C-z */
    signal(SIGTSTP, handler);
    printf("%u\n", sleep(5));   /* sleep 返回剩余的秒数 */
}
