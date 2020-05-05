/* 66-fork-fork-fork.c --- ex24-1 */
#include <stdio.h>
#include <unistd.h>
int
main()
{
    pid_t parentPid = getpid();

    fork();
    fork();
    fork();

    if (getpid() == parentPid) {
        sleep(1);
        printf("Pid=%ld\n", (long) getpid());
    } else {
        printf("Pid=%ld, Parent Pid=%ld\n",
               (long) getpid(), (long) getppid());
    }
}
