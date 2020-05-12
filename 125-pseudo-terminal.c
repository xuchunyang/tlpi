/* 125-pseudo-terminal.c --- 伪终端 */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/_select.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <sys/ttycom.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>


static void
die(char* s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

struct termios ttyOrig;

static void
cleanup()
{
    if (tcsetattr(STDIN_FILENO, TCSANOW, &ttyOrig) == -1)
        die("tcsetattr");
}

static void
ttySetRaw()
{
    struct termios t = ttyOrig;
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

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &t) == -1)
        die("tcsetattr");
}

int
main()
{
    int masterFd = posix_openpt(O_RDWR | O_NOCTTY); /* 打开 pty 主设备 */
    if (masterFd == -1) die("posix_openpt");

    if (grantpt(masterFd) == -1) { /* 保证 pty 从设备的访问权限 */
        perror("grantpt");
        close(masterFd);
        exit(EXIT_FAILURE);
    }

    if (unlockpt(masterFd) == -1) { /* 解锁 pty 从设备 */
        die("unlockpt");
    }

    char *slname = ptsname(masterFd); /* 获得 pty 从设备的名称 */
    if (slname == NULL) {
        perror("ptsname");
        close(masterFd);
        exit(EXIT_FAILURE);
    }

    printf("成功获得 pty 从设备： %s\n", slname);

    if (tcgetattr(STDIN_FILENO, &ttyOrig) == -1) {
        perror("tcgetattr");
        _exit(EXIT_FAILURE);
    }

    struct winsize ws;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        _exit(EXIT_FAILURE);
    }

    pid_t childPid = fork();
    if (childPid == -1) {
        perror("fork");
        close(masterFd);
        exit(EXIT_FAILURE);
    }

    if (childPid == 0) {        /* Child */
        if (setsid() == -1) {   /* Start a new session */
            perror("setsid");
            _exit(EXIT_FAILURE);
        }

        close(masterFd);        /* Not needed in child */

        int slavedFd = open(slname, O_RDWR); /* Become controlling tty */
        if (slavedFd == -1) {
            perror("open");
            _exit(EXIT_FAILURE);
        }

#ifdef TIOCSCTTY                /* Acquire controlling tty on BSD */
        if (ioctl(slavedFd, TIOCSCTTY, 0) == -1) {
            perror("ioctl");
            _exit(EXIT_FAILURE);
        }
#endif

        if (tcsetattr(slavedFd, TCSANOW, &ttyOrig) == -1) {
            perror("tcsetattr");
            _exit(EXIT_FAILURE);
        }

        if (ioctl(slavedFd, TIOCSWINSZ, &ws) == -1) {
            perror("ioctl");
            _exit(EXIT_FAILURE);
        }

        if (dup2(slavedFd, STDIN_FILENO) == -1) {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }
        if (dup2(slavedFd, STDOUT_FILENO) == -1) {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }
        if (dup2(slavedFd, STDERR_FILENO) == -1) {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }

        if (slavedFd > STDERR_FILENO) /* Safety check */
            close(slavedFd);          /* No longer need this fd */

        execlp("bash", "bash", NULL);
        perror("execlp");       /* If we get here, something went wrong */
        _exit(EXIT_FAILURE);
    } else {                    /* Parent */

        /* Parent: reply data between terminal and pty master */

        ttySetRaw();

        if (atexit(cleanup) == -1) {
            perror("atexit");
            exit(EXIT_FAILURE);
        }

#define BUF_SIZE 256
        char buf[BUF_SIZE];

        while(1) {
            fd_set inFds;
            FD_ZERO(&inFds);
            FD_SET(STDIN_FILENO, &inFds);
            FD_SET(masterFd, &inFds);

            if (select(masterFd + 1, &inFds, NULL, NULL, NULL) == -1) {
                perror("select");
                exit(EXIT_FAILURE);
            }

            if (FD_ISSET(STDIN_FILENO, &inFds)) { /* stdin --> pty */
                ssize_t numRead = read(STDIN_FILENO, buf, BUF_SIZE);
                if (numRead <= 0)
                    exit(EXIT_SUCCESS);

                if (write(masterFd, buf, numRead) != numRead) {
                    printf("partial/failed write (masterFd)\n");
                    exit(EXIT_FAILURE);
                }
            }

            if (FD_ISSET(masterFd, &inFds)) { /* pty --> stdout */
                ssize_t numRead = read(masterFd, buf, BUF_SIZE);
                if (numRead <= 0)
                    exit(EXIT_SUCCESS);

                if (write(STDOUT_FILENO, buf, numRead) != numRead) {
                    printf("partial/failed write (STDOUT_FILENO)");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}
