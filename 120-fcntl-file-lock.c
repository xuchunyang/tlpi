/* 120-fcntl-file-lock.c --- fcntl 文件锁 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

int
main()
{
    int fd = open("tfile", O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Adding writing lock...\n");
    struct flock lock = {
        .l_type       = F_WRLCK,
        .l_whence     = SEEK_SET,
        .l_start      = 0,
        .l_len        = 0,      /* until EOF */
    };
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    snprintf(buf, sizeof buf, "Written by PID %ld\n", (long) getpid());
    printf("Writing '%s' to file...\n", buf);
    sleep(5);
    write(fd, buf, strlen(buf));
}
