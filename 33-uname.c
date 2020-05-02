/* 33-uname.c --- uname */
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>

int
main()
{
    struct utsname un;
    if (uname(&un) == -1) {
        perror("uname");
        exit(EXIT_FAILURE);
    }
    puts(un.machine);
    puts(un.nodename);
    puts(un.release);
    puts(un.sysname);
    puts(un.version);
}
