/* 105-ls-wc.c --- Implement 'ls | wc -l' using pipe(2) */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
errExit(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

int
main()
{
    int fds[2];
    if (pipe(fds) == -1)
        errExit("pipe");

    int pid = fork();
    if (pid == -1)
        errExit("pipe");
    else if (pid == 0) {        /* child */
        if (close(fds[0]))      /* close read */
            errExit("close");

        if (fds[1] != STDOUT_FILENO) { /* p737, 防御性 */
            if (dup2(fds[1], STDOUT_FILENO) == -1)
                errExit("dup2");
            if (close(fds[1]) == -1) /* 关闭 solo fd */
                errExit("close");
        }

        execlp("ls", "ls", NULL);
        errExit("execlp ls");
        /* NOTE 结果是对的，但底气不足。wc 进程应该是会等待 ls 进程结束的，这一点无疑 */
        // execlp("find", "find", ".", NULL);
        // execlp("yes", "yes", ".", NULL);
    } else {                    /* parent */
        if (close(fds[1]))      /* close write */
            errExit("close");

        if (fds[0] != STDIN_FILENO) {
            if (dup2(fds[0], STDIN_FILENO) == -1)
                errExit("dup2");
            if (close(fds[0]) == -1)
                errExit("close");
        }

        execlp("wc", "wc", "-l", NULL);
        errExit("execlp wc");
    }
}
