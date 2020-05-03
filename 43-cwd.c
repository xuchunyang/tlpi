/* 43-cwd.c --- current working directory */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
pwd()
{
    /* macOS, 1024 */
    // printf("PATH_MAX = %d\n", PATH_MAX);
    char buf[PATH_MAX];
    if (getcwd(buf, sizeof buf) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    printf("CWD: %s\n", buf);
}

int
main()
{
    pwd();

    if (chdir("/tmp") == -1) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    if (chdir("/non-exist") == -1) {
        perror("chdir");
    }

    pwd();
}
