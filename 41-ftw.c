/* 40-ftw.c --- file tree walker */
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int
display_info(const char* fpath,
             const struct stat *sb, /* stat(2) */
             int tflag,             /* file types */
             struct FTW* ftwbuf     /* file base, dir depth */
             )
{
    if (tflag == FTW_NS) {
        printf("ERROR: sb 不用\n");
    }
    printf("%s\t%lld\t%d\t%d\n", fpath, sb->st_size,
           /* 最后一个 / 相对于开头的偏移 */
           ftwbuf->base,
           /* 深度 */
           ftwbuf->level);
    return 0;
}

int
main(int argc, char* argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s dirname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int flags =
        FTW_PHYS                /* 不处理符号链接 */
        ;
    if (nftw(argv[1], display_info, 20, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
}
