/* 119-flock.c --- 互斥锁 exclusive flock(2) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/file.h>           /* flock */

char*
now(char* fmt)
{
    time_t t = time(NULL);
    if (t == (time_t) -1) {
        perror("time");
        exit(EXIT_FAILURE);
    }

    struct tm* tt = localtime(&t);
    if (tt == NULL) {
        perror("localtime");
        exit(EXIT_FAILURE);
    }

    static char buf[1024];
    if (strftime(buf, sizeof buf, fmt, tt) == 0) {
        perror("strftime");
        exit(EXIT_FAILURE);
    }

    return buf;
}

int
main(int argc, char *argv[])
{
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s file msg\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("%s Opening %s\n", now("%T"), argv[1]);
    int fd = open(argv[1], O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("%s Locking %s\n", now("%T"), argv[1]);
    if (flock(fd, LOCK_EX) == -1) {
        perror("flock");
        exit(EXIT_FAILURE);
    }

    int sleep_time = 5;
    printf("Sleeping for %d seconds\n", sleep_time);
    sleep(5);

    printf("%s Writing '%s' to %s\n", now("%T"), argv[2], argv[1]);
    if (write(fd, argv[2], strlen(argv[2])) != (ssize_t) strlen(argv[2])) {
        perror("write partial/error");
        exit(EXIT_FAILURE);
    }
}
