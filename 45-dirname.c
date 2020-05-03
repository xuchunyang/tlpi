/* 45-dirname.c --- dirname & basename */
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>

char* foo()
{
    /* 所谓静态分配 */
/* 45-dirname.c:8:12: warning: address of stack memory associated with local variable 'buf' */
/*       returned [-Wreturn-stack-address] */
    static char buf[10] = "hello";
    printf("'%s'\n", buf);
    return buf;
}

int
main(int argc, char* argv[])
{
    char* s = foo();
    printf("'%s'\n", s);

    char* dir = dirname("/path/to/file");
    if (dir == NULL) {
        perror("dirname");
        exit(EXIT_FAILURE);
    }
    puts(dir);

    dirname("/hello/world");
    // 所谓静态分配
    puts(dir);

    char* base = basename("/path/to/file");
    if (base == NULL) {
        perror("basename");
        exit(EXIT_FAILURE);
    }
    puts(base);
}
