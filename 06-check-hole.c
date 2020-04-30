/* 06-check-hole.c --- 检查文件系统支不支持 hole */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main()
{
#ifdef _PC_MIN_HOLE_SIZE
    long size = pathconf("Makefile", _PC_MIN_HOLE_SIZE);
    if (size == -1) {
        perror("pathconf(_PC_MIN_HOLE_SIZE)");
        exit(EXIT_FAILURE);
    }
    /* 文件系统支持 hole？ */
    printf("_PC_MIN_HOLE_SIZE: %ld\n", size);
#else
    printf("_PC_MIN_HOLE_SIZE 没有定义，它只在 BSD 系统上才有定义，Linux 上没有\n");
#endif
}
