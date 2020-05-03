/* 39-readdir.c --- readdir */

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

static void ls(DIR* dirp);

int
main(int argc, char* argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s dirname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR* dirp = opendir(argv[1]);
    if (dirp == NULL) {
        printf("opendir failed\n");
        exit(EXIT_FAILURE);
    } else
        printf("opendir %s success\n", argv[1]);

    if (closedir(dirp) == -1) {
        perror("closedir");
        exit(EXIT_FAILURE);
    }

    /* fopendir 只需要 read 权限吧? */
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    dirp = fdopendir(fd);
    if (dirp == NULL) {
        printf("fopendir failed\n");
        exit(EXIT_FAILURE);
    } else
        printf("fopendir %s (fd=%d) success\n", argv[1], fd);

    ls(dirp);

    rewinddir(dirp);

    ls(dirp);

    if (closedir(dirp) == -1) {
        perror("closedir");
        exit(EXIT_FAILURE);
    }
}

static void ls(DIR* dirp)
{
    struct dirent* dp;
    errno = 0;
    while ((dp = readdir(dirp))) {
        printf("%-20s:\t", dp->d_name);
        switch (dp->d_type) {
        case DT_REG:  printf("regular file\n");      break;
        case DT_DIR:  printf("directory\n");         break;
        case DT_LNK:  printf("symbolic link\n");     break;
        case DT_FIFO: printf("FIFO or pipe \n");     break;
        default:      printf("unknown file type\n"); break;
        }
    }
    if (errno != 0) {
        perror("readdir");
    }
}
