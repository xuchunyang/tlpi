/* 00-glibc-version.c --- 确定 glibc 版本 */
#include <stdio.h>
#include <gnu/libc-version.h>

int main(void)
{
#ifdef __GLIBC__
    printf("__GLIBC__: %d\n", __GLIBC__);
#endif
#ifdef __GLIBC_MINOR__
    printf("__GLIBC_MINOR__: %d\n", __GLIBC_MINOR__);
#endif

    printf("GNU libc version: %s\n", gnu_get_libc_version());
    printf("GNU libc release: %s\n", gnu_get_libc_release());
}