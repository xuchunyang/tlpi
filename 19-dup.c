/* 19-dup.c --- check dup */

#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>

static void
die(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

static int
getAccessMode(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) die("fcntl");
    int accessMode = flags & O_ACCMODE;
    return accessMode;
}

int
main()
{
    char template[] = "testXXXXXX";
    int fd = mkstemp(template);
    if (fd == -1) die("mkstemp");

    int fd2 = dup(fd);

    off_t offset = 100;
    lseek(fd, offset, SEEK_SET);

    off_t offset2 = lseek(fd2, 0, SEEK_CUR);
    if (offset2 != offset) {
        printf("ERROR: %lld != %lld",
               (long long) offset,
               (long long) offset2);
    } else {
        printf("共享文件偏移\n");
    }

    int accessMode1 = getAccessMode(fd);
    int accessMode2 = getAccessMode(fd2);
    if (accessMode1 != accessMode2) {
        printf("ERROR: 文件访问模式不同: %d != %d",
               accessMode1,
               accessMode2);
    } else {
        printf("文件访问模式相同: %d\n", accessMode1);
        printf("%d %d %d\n",
               O_RDONLY,
               O_WRONLY,
               O_RDWR);
    }

    if (close(fd) == -1) die("close");
}
