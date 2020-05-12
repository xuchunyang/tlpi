/* 124-terminal-window-size.c --- 终端窗口大小 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/ttycom.h>
#include <unistd.h>

static void
handler(int sig __attribute__((unused)))
{

}

int
main()
{
    if (signal(SIGWINCH, handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    while (1) {
        pause();

        struct winsize ws;
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1) {
            perror("ioctl");
            exit(EXIT_FAILURE);
        }
        printf("window size: %d rows * %d columns\n", ws.ws_row, ws.ws_col);
    }
}
