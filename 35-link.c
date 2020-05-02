/* 35-link.c --- link(2) and unlink(2), hard link 的创建和删除 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    if (argc != 2 || strcmp(argv[0], "--help") == 0) {
        printf("usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

#define SUFFIX ".copy"

    size_t buf_size = strlen(argv[0]) + strlen(SUFFIX) + 1;
    char* buf = malloc(buf_size);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    if (snprintf(buf, buf_size, "%s" SUFFIX, argv[1]) < 0) {
        perror("snprintf");
        exit(EXIT_FAILURE);
    }

    printf("ln %s %s...\n", argv[1], buf);

    if (link(argv[1], buf) == -1) {
        perror("link");
        exit(EXIT_FAILURE);
    }

    system("ls -li");

    if (unlink(buf) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
}
