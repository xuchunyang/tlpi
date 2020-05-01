/* 30-locale.c --- locale */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <locale.h>

int
main()
{
    time_t t = time(0);
    struct tm *tt = localtime(&t);
    if (tt == NULL) {
        perror("localtime");
        exit(EXIT_FAILURE);
    }

#define BUF_SIZE 1024

    char buf[BUF_SIZE];
    assert(strftime(buf, BUF_SIZE,
                    "%H:%M:%S %A, %d %B %Y %Z", tt));
    printf("'%s'\n", buf);

    /* 继承环境变量 */
    errno = 0;
    if (setlocale(LC_ALL, "") == NULL) {
        perror("setlocale");
        printf("setlocale failed\n");
        exit(EXIT_FAILURE);
    }
    assert(strftime(buf, BUF_SIZE,
                    "%H:%M:%S %A, %d %B %Y %Z", tt));
    printf("'%s'\n", buf);
}
