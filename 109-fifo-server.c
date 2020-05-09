/* 109-fifo-server.c --- plus one server */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_FIFO "/tmp/server-fifo"

struct request {
    pid_t pid;
    int num;
};

struct response {
    int num;
};

static void
errExit(char* s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

int
main()
{
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR) == -1 && errno != EEXIST)
        errExit("mkfifo");
    errno = 0;

    printf("sizeof(struct request) = %zu bytes\n", sizeof(struct request));

    int fd = open(SERVER_FIFO, O_RDONLY);
    if (fd == -1)
        errExit("open");

    while(1) {
        struct request req;
        ssize_t numRead = read(fd, &req, sizeof req);
        if (numRead == -1)
            errExit("read");
        else if ((size_t) numRead < sizeof req) {
            // printf("read partially; numRead: %zd\n", numRead);
            continue;
        }

        printf("received a request, %d + 1 = ?\n", req.num);

        char buf[100];
        snprintf(buf, sizeof buf, "/tmp/client-fifo-%ld", (long) req.pid);
        int clientFd = open(buf, O_WRONLY);
        if (clientFd == -1)
            errExit("open");

        printf("sending response\n");

        struct response resp = {
            .num = req.num + 1,
        };
        ssize_t numWritten = write(clientFd, &resp, sizeof resp);
        if (numWritten == -1)
            errExit("write");

        if ((size_t) numWritten < sizeof resp)
            printf("write partially; numWritten: %zd\n", numWritten);

        if (close(clientFd) == -1)
            errExit("close");
    }
}
