/* 15-tmpfile.c --- 临时文件 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main()
{
    char template[] = "/tmp/somethingXXXXXX";
    int fd = mkstemp(template);
    if (fd == -1) {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }
    if (unlink(template) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
    printf("Generated temp file: %s\n", template);
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
