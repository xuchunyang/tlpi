/* 12-readv.c --- readv */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int
main()
{
    int fd = STDIN_FILENO;
    struct iovec iov[3];
    const int iovcnt = sizeof iov / sizeof *iov;

    char buf1[1];
    iov[0].iov_base = buf1;
    iov[0].iov_len = sizeof buf1;

    char buf2[2];
    iov[1].iov_base = buf2;
    iov[1].iov_len = sizeof buf2;

    char buf3[3];
    iov[2].iov_base = buf3;
    iov[2].iov_len = sizeof buf3;

    /* printf("%lu\n", sizeof buf1); */
    /* printf("%lu\n", sizeof buf2); */
    /* printf("%lu\n", sizeof buf3); */

    ssize_t totRequired = 0;
    for (size_t i = 0; i < iovcnt; i++)
        totRequired += iov[i].iov_len;

    ssize_t numRead = readv(fd, iov, iovcnt);
    if (numRead == -1) {
        perror("readv");
        exit(EXIT_FAILURE);
    }

    if (numRead < totRequired)
        printf("Read fewer bytes than requested\n");
    else
        for (size_t i = 0; i < iovcnt; i++) {
            printf("%zu. '", i);
            fflush(stdout);
            ssize_t numWritten = write(STDOUT_FILENO, iov[i].iov_base, iov[i].iov_len);
            if (numWritten == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            fflush(stdout);
            printf("'\n");
        }

    printf("total bytes requested: %ld, byte read: %ld\n", totRequired, numRead);
}
