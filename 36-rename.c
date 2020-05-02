/* 36-rename.c --- rename(2) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char* argv[])
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s oldfile newfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (rename(argv[1], argv[2]) == -1) {
        perror("rename");
        exit(EXIT_FAILURE);
    }
}
