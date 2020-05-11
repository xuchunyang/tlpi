/* 121-terminal-getpass.c --- 读取一个字符串（密码，关闭 Echo） */
#include <stdio.h>
#include <stdlib.h>
#include <sys/termios.h>
#include <unistd.h>
#include <termios.h>

static struct termios orig;

static void
restore_terminal()
{
    // printf("Restoring ECHO...\n");
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

int
main()
{
    setbuf(stdout, NULL);
    printf("Enter password: ");

    /* Turn off Echo */

    // printf("Turn off echo...\n");
    if (tcgetattr(STDIN_FILENO, &orig) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    struct termios tp = orig;
    tp.c_lflag &= ~ECHO;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    if (atexit(restore_terminal) == -1) {
        perror("atexit");
        exit(EXIT_FAILURE);
    }

    char buf[1024] = {0};
    ssize_t numRead = read(STDIN_FILENO, buf, sizeof buf);
    if (numRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    if (numRead > 0 && buf[numRead-1] == '\n') {
        buf[numRead-1] = '\0';
    }
    printf("\nYour password is '%s'\n", buf);
}
