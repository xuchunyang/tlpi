/* 03-copy.c --- 4-1 使用 I/O 系统调用 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h> // S_IRUSR

#ifndef BUF_SIZE
# define BUF_SIZE 1024
#endif

int
main(int argc, char* argv[])
{
    if (argc != 3 || !strcmp(argv[1], "--help")) {
        fprintf(stderr, "Usage: %s old-file new-file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Open input and output files */

    int inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        fprintf(stderr, "opening file %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    int openFlags = O_CREAT | O_TRUNC | O_WRONLY;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int outputFd = open(argv[2], openFlags, filePerms);
    if (inputFd == -1) {
        fprintf(stderr, "opening file %s: %s\n", argv[2], strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Transfer data until we encounter end of input or an error */

    char buffer[BUF_SIZE];
    ssize_t numRead;
    while((numRead = read(inputFd, buffer, BUF_SIZE)) > 0)
        if (write(outputFd, buffer, numRead) != numRead) {
            perror("couldn't write the whole buffer");
            exit(EXIT_FAILURE);
        }
    if (numRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    if (close(inputFd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    if (close(outputFd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
