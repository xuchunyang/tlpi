/* 13-writev.c --- writev */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

int main()
{
    int fd = STDERR_FILENO;
    struct iovec iov[3];

    char c = 'X';
    iov[0].iov_base = &c;
    iov[0].iov_len = sizeof c;

    char *s = "hello";
    iov[1].iov_base = s;
    iov[1].iov_len = strlen(s);

    unsigned n = 0xdeadbeef;
    iov[2].iov_base = &n;
    iov[2].iov_len = sizeof n;

    ssize_t totRequired = 0;
    int iovcnt = sizeof iov / sizeof *iov;
    for (int i = 0; i < iovcnt; i++)
        totRequired += iov[i].iov_len;

    ssize_t numWritten = writev(fd, iov, iovcnt);
    if (numWritten == -1) {
        perror("writev");
        exit(EXIT_FAILURE);
    }

    if (numWritten < totRequired)
        printf("fewer bytes wrote\n");

    printf("totRequired: %lu, numWritten: %lu\n", totRequired, numWritten);
}
