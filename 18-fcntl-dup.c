/* 18-fcntl-dup.c --- ex5-4 用 fcntl 实现 dup */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <assert.h>

static int
mydup(int fd)
{
    return fcntl(fd, F_DUPFD, fd);
}

static int
mydup2(int oldfd, int newfd)
{
    if (fcntl(oldfd, F_GETFL) == -1) {
        /* oldfd 无效 */
        errno = EBADF;
        return -1;
    }
    return fcntl(oldfd, F_DUPFD, newfd);
}

int
main()
{
    int fd = dup(STDOUT_FILENO);
    if (fd == -1) {
        perror("dup");
        exit(EXIT_FAILURE);
    }
    printf("fd = %d\n", fd);
    assert(write(fd, "hello", 5) == 5);

    {
        int fd = mydup(STDOUT_FILENO);
        if (fd == -1) {
            perror("mydup");
            exit(EXIT_FAILURE);
        }
        printf("fd = %d\n", fd);
        assert(write(fd, "hello", 5) == 5);
    }

    {
        if (dup2(19, 19) == -1) {
            perror("dup2");
            errno = 0;
        }

        if (mydup2(1, 10) == -1) {
            perror("mydup2");
        }

    }
}
