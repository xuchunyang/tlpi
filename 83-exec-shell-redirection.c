/* 83-exec-shell-redirection.c --- 利用 exec 继承 file descriptors 特性实现重定向 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_s_ifmt.h>
#include <sys/fcntl.h>
#include <unistd.h>

int
main()
{
    /* 实现 ls -l > dir.txt */
    int fd = open("dir.txt",
                  O_CREAT | O_WRONLY | O_TRUNC,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (fd != STDOUT_FILENO) {
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd);
    }
    execlp("ls", "ls", "-l", NULL);
}

