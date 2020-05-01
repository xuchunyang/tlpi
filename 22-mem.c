/* 22-mem.c --- memory layouy */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

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

    printf("\n\nProgram break:\n");
    printf("    initial program break     %10p\n", sbrk(0));
#define BUF_SIZE 4096
    /* ex7-1 */
    for (size_t i = 0; i < 1000; i++) {
      char* buf = malloc(BUF_SIZE);
      assert(buf);
      printf("    program break             %10p\n", sbrk(0));
      buf[BUF_SIZE - 1] = 'x';
      printf("    program break             %10p\n", sbrk(0));
    }

    /* ex6-1 */
    static char mbuf[10240000] = {0}; /* 4.2k 没初始化的数据段，几乎不占空间 */
    // static char mbuf[10240000] = {1}; /* 10MB 初始化的数据段，占空间 */

    (void) mbuf;
}
