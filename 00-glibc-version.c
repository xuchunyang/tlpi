/* 00-glibc-version.c --- 确定 glibc 版本 */
#ifdef __GLIBC__
#include <stdio.h>
#include <gnu/libc-version.h>
int main(void)
{
    printf("__GLIBC__: %d\n", __GLIBC__);
    printf("__GLIBC_MINOR__: %d\n", __GLIBC_MINOR__);
    printf("GNU libc version: %s\n", gnu_get_libc_version());
    printf("GNU libc release: %s\n", gnu_get_libc_release());
}
#else
int main(void){}
#endif
