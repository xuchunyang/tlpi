/* 37-symbolic-link.c --- Symbolic link */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char buf[1024];

    snprintf(buf, sizeof buf, "%s.symbolic-link", argv[1]);
    if (symlink(argv[1], buf) == -1) {
        perror("symlink");
        exit(EXIT_FAILURE);
    }
    char* linkpath = strdup(buf);
    if (linkpath == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    snprintf(buf, sizeof buf, "ls -li %s*", argv[1]);
    system(buf);

    ssize_t numBytes = readlink(linkpath, buf, sizeof buf);
    if (numBytes == -1) {
        perror("readlink");
        exit(EXIT_FAILURE);
    }
    buf[numBytes] = 0;
    printf("'%s' -> '%s'\n", linkpath, buf);

}
