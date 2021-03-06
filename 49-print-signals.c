/* 49-print-signals.c --- Print signals */

#include <signal.h>
#include <string.h>
#include <stdio.h>

int
main()
{
    printf("'%s'\n", strsignal(SIGHUP));
    printf("'%s'\n", strsignal(SIGINT));

    printf("Description: '%s': %d\n", sys_siglist[SIGHUP], SIGHUP);
#ifdef __APPLE__
    printf("NAME: '%s': %d\n", sys_signame[SIGHUP], SIGHUP);
#endif

    psignal(SIGINT, "SIGINT");

    puts("\n---\n");
    for (int i = 1; i < 32; i++)
        puts(strsignal(i));

    /* 32 */
    printf("%zu\n",
           sizeof sys_siglist / sizeof *sys_siglist);

    printf("Number of signals: NSIG=%d\n", NSIG);
}
