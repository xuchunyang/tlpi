/* 29-time.c --- time */
#define _BSD_SOURCE             /* tm_gmtoff, tm_zone */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>

int
main()
{
    /* 日历时间 */

    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        perror("gettimeofday");
        exit(EXIT_FAILURE);
    }
    printf("sec: %ld, usec: %d\n", tv.tv_sec, tv.tv_usec);

    time_t t = time(NULL);
    printf("sec: %ld\n", t);

    /* gettimeofday 是系统调用，而 time 是库函数
       struct timeval 比 time_t 更精确
    */
    printf("'%s'\n", ctime(&t));

    /* struct tm 叫做 broken-down time
       gmtime 使用 UTC
    */
    struct tm* tt = gmtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d %ld\n",
           tt->tm_year + 1900,
           tt->tm_mon + 1,
           tt->tm_mday,
           tt->tm_hour,
           tt->tm_min,
           tt->tm_sec,
           tt->tm_gmtoff
        );
        printf("Seconds since Epoch: %ld\n",
               timegm(tt));

    {
        /* localtime 受 TZ 环境变量影响 */
        struct tm* tt = localtime(&t);
        printf("%d-%02d-%02d %02d:%02d:%02d %ld %s\n",
               tt->tm_year + 1900,
               tt->tm_mon + 1,
               tt->tm_mday,
               tt->tm_hour,
               tt->tm_min,
               tt->tm_sec,
               tt->tm_gmtoff,
               tt->tm_zone
            );
        printf("'%s'\n", asctime(tt));

        printf("Seconds since Epoch: %ld\n",
               mktime(tt));

#define BUF_SIZE 1024
        char buf[BUF_SIZE];
        if (strftime(buf, BUF_SIZE, "%Y-%m-%d %H:%M:%S %z %Z", tt) == 0) {
            // perror("strftime");
            printf("ERROR: strftime returns 0\n");
            exit(EXIT_FAILURE);
        }
        printf("'%s'\n", buf);

        {
            struct tm tt = {0};
            assert(strptime("2020-05-01 19:13:52 +0800", "%Y-%m-%d %H:%M:%S %z", &tt));
            puts(asctime(&tt));
        }
    }
}
