/* 123-terminal-raw-mode.c --- Raw Mode */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/termios.h>
#include <termios.h>
#include <unistd.h>

static void
die(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

int
main()
{
    int fd = STDIN_FILENO;

    struct termios t0;
    if (tcgetattr(fd, &t0) == -1)
        die("tcgetattr");

    struct termios t = t0;

    t.c_lflag &= ~(
        ICANON                  /* 规范模式 */
        | ECHO                  /* 回显输入 */
        | ISIG                  /* 字符信号 */
        | IEXTEN                /* 输入字符扩展处理 */
        );

    t.c_iflag &= ~(
        ICRNL                   /* \r -> \n */
        | INLCR                 /* \n -> \r */
        | IXON
        );

    t.c_oflag &= ~OPOST;

    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSAFLUSH, &t) == -1)
        die("tcsetattr");

    setbuf(stdout, NULL);

    while(1) {
        char ch;
        ssize_t n = read(fd, &ch, 1);
        if (n == -1)
            die("read");

        if (n == 0)
            break;

        if (isalnum(ch))
            putchar(ch);
        else if (ch == '\n' || ch == '\r')
            putchar(ch);
        else if (iscntrl(ch))
            printf("^%c", ch ^ 64);
        else
            putchar('*');

        if (ch == 'q')
            break;
    }

    if (tcsetattr(fd, TCSAFLUSH, &t0) == -1)
        die("tcsetattr");
}
