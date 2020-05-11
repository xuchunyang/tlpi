/* 122-terminal-intr-key.c --- 修改 INTR 的按键，默认 C-c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/termios.h>
#include <termios.h>
#include <unistd.h>

int
main()
{
    struct termios ts;
    if (tcgetattr(STDIN_FILENO, &ts) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    /* C-c  3 */
    /* C-l 12 */
    ts.c_cc[VINTR] = 12;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ts) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}
