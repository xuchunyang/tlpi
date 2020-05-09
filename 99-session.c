/* 99-session.c --- Session */
#include <stdio.h>
#include <unistd.h>

int
main()
{
    printf("PID=%ld PPID=%ld PGID=%ld Session ID = %ld\n",
           (long) getpid(),
           (long) getppid(),
           (long) getpgrp(),
           (long) getsid(0));
}
