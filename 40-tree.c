/* 40-tree.c --- tree */

#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

static void tree(const char* rootdir, int depth)
{
    DIR* dirp = opendir(rootdir);
    if (dirp == NULL) {
        printf("opendir failed\n");
        exit(EXIT_FAILURE);
    } else
        // printf("open %s success\n", rootdir);
        ;

    struct dirent* dp;
    errno = 0;
    while ((dp = readdir(dirp))) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;;
        printf("%*s%s\n", 4*depth, "", dp->d_name);
        if (dp->d_type == DT_DIR) {
            char buf[PATH_MAX];
            snprintf(buf, sizeof buf, "%s/%s", rootdir, dp->d_name);
            tree(buf, depth + 1);
        }
    }
    if (errno != 0) {
        perror("readdir");
        exit(EXIT_FAILURE);
    }

    if (closedir(dirp) == -1) {
        perror("closedir");
        exit(EXIT_FAILURE);
    }
}

int
main(int argc, char* argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s dirname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    tree(argv[1], 0);
}
