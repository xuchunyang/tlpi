/* 08-fcntl.c --- 文件控制操作 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>

int
main()
{
    int fd3 = open("Makefile", O_WRONLY);
    if (fd3 == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    // printf("fd3 = %d\n", fd3);

    for (int fd = 0; fd <= fd3; fd++) {
        int flags = fcntl(fd, F_GETFL);
        if (flags == -1) {
            perror("fcntl");
            exit(EXIT_FAILURE);
        }
        int accessMode = flags & O_ACCMODE;
        // printf("accessMode = %d\n", accessMode);
        switch (accessMode) {
        case O_WRONLY:
            printf("%d writeonly\n", fd);
            break;
        case O_RDONLY:
            printf("%d readonly\n", fd);
            break;
        case O_RDWR:
            printf("%d read and write\n", fd);
            break;
        }
    }

    if (close(fd3) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
