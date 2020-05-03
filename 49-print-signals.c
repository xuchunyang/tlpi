/* 49-print-signals.c --- Print signals */

#include <signal.h>
#include <string.h>
#include <stdio.h>

int
main()
{
    printf("'%s'\n", strsignal(SIGHUP));
    printf("'%s'\n", strsignal(SIGINT));

    printf("'%s': %d\n", sys_siglist[SIGHUP], SIGHUP);

    psignal(SIGINT, "SIGINT");

    puts("\n---\n");
    for (int i = 1; i < 32; i++)
        puts(strsignal(i));
}
