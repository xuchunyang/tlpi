/* 98-controling-terminal.c --- 当前进程的控制终端 */
#include <stdio.h>
#include <unistd.h>

int
main()
{
    printf("控制终端: %s\n", ctermid(NULL));

    int fds[] = {STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO};
    for (size_t i = 0; i < sizeof fds / sizeof *fds; i++) {
        printf("Is %d a terminal device? %s\n",
               fds[i],
               isatty(fds[i]) ? "Y" : "N");
        if (isatty(fds[i]))
            printf("\tttyname of stdin: %s\n", ttyname(fds[i]));
    }
}
