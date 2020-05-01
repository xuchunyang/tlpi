/* 31-clock.c --- 系统时间 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <sys/times.h>
#include <math.h>

int
main()
{
    long clock_per_sec = sysconf(_SC_CLK_TCK);
    if (clock_per_sec == -1) {
        perror("sysconf");
        exit(EXIT_FAILURE);
    }
    printf("每秒钟的 clock 次数: %ld\n", clock_per_sec);
    printf("每秒钟的 clock 次数: %ld\n", (long) CLOCKS_PER_SEC);

    /* clock 返回 CPU 时间，包括用户和系统，不受 sleep 影响 */
    assert(sleep(1) == 0);

    double sum = 0;
    for (size_t i = 0; i < 100000; i++) {
        sum += sin(i) + 1;
    }
    printf("%g\n", sum);

    clock_t ct = clock();
    if (ct == (clock_t) -1) {
        printf("ERROR: clock\n");
        exit(EXIT_FAILURE);
    }
    printf("%g\n", (double) ct / CLOCKS_PER_SEC);

    struct tms tt = { 0 };
    if (times(&tt) == (clock_t) -1) {
        printf("ERROR: times\n");
        exit(EXIT_FAILURE);
    }
    printf("User   CPU time: %lu\n", tt.tms_utime);
    printf("System CPU time: %lu\n", tt.tms_stime);
}
