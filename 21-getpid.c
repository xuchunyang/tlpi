/* 21-getpid.c --- getpid */

#include <stdio.h>
#include <unistd.h>

int
main()
{
    pid_t pid = getpid();
    printf("PID: %d\n", pid);

    pid_t ppid = getppid();
    printf("PPID: %d\n", ppid);
}
