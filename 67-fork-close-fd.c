/* 67-fork-close-fd.c --- ex24-2 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int
main()
{
    fork();
    switch (fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0:
        close(STDIN_FILENO);
        _exit(EXIT_SUCCESS);

    default:
        sleep(3);
        char buf[3];
        read(STDIN_FILENO, buf, sizeof buf);
    }
}
