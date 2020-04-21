/* 01-strerror.c --- strerror 的返回值是静态分配的到底是什么意思？*/
#include <string.h>
#include <stdio.h>

int
main(void)
{
    char *s1 = strerror(1);
    char *s2 = strerror(2);
    char *s3 = strerror(3);
    char *s4 = strerror(4);
    printf("%s\n", s1);
    printf("%s\n", s2);
    printf("%s\n", s3);
    printf("%s\n", s4);
}
