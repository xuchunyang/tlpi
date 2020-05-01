/* 22-mem.c --- memory layouy */
#include <stdio.h>
#include <unistd.h>

#ifdef __APPLE__
#include <mach-o/getsect.h>
#endif

int
main()
{
    /* page 96 */
#ifdef __APPLE__
    printf("First address past:\n");
    printf("    program text (etext)      %10p\n", (void*)get_etext());
    printf("    initialized data (edata)  %10p\n", (void*)get_edata());
    printf("    uninitialized data (end)  %10p\n", (void*)get_end());
#else
    extern char etext, edata, end;
    printf("First address past:\n");
    printf("    program text (etext)      %10p\n", &etext);
    printf("    initialized data (edata)  %10p\n", &edata);
    printf("    uninitialized data (end)  %10p\n", &end);
#endif

    printf("    program break             %10p\n", sbrk(0));

    /* ex6-1 */
    static char mbuf[10240000] = {0}; /* 4.2k 没初始化的数据段，几乎不占空间 */
    // static char mbuf[10240000] = {1}; /* 10MB 初始化的数据段，占空间 */

    (void) mbuf;
}
