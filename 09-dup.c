/* 09-dup.c --- 文件描述符 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>
int
main()
{
    /* free file descriptor 2 */
    if (close(2) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    int fd = open("stderr.log", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (fd != 2) {
        printf("预期是 2，实际是 %d\n", fd);
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "stderr msg\n");
    if (write(fd, "hello world", 11) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
