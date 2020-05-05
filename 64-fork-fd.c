/* 64-fork-fd.c --- fork and file descriptor */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int
main()
{
    char template[] = "/tmp/testXXXXXX";
    int fd = mkstemp(template);
    if (fd == -1) {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }
    printf("Generated temp file: %s\n", template);

    printf("File offset before fork(): %lld\n", lseek(fd, 0, SEEK_CUR));

    switch (fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0:
        if (lseek(fd, 1000, SEEK_SET) == -1) {
            perror("lseek");
            exit(EXIT_FAILURE);
        }
        _exit(EXIT_SUCCESS);

    default:
        if (wait(0) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
        printf("Child has exited\n");
        printf("File offset in parent: %lld\n", lseek(fd, 0, SEEK_CUR));
    }
}
