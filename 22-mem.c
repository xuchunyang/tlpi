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
}
