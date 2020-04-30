/* 22-mem.c --- memory layouy */
#include <stdio.h>
int
main()
{
#ifdef NOT_WORKING
    /* page 96 */
    extern char etext, edata, end;
    printf("%d\n", etext);
#endif

    /* ex6-1 */
    static char mbuf[10240000] = {0}; /* 4.2k 没初始化的数据段，几乎不占空间 */
    // static char mbuf[10240000] = {1}; /* 10MB 初始化的数据段，占空间 */

    (void) mbuf;
}
