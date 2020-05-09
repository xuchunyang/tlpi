/* 110-fifo-client.c --- client to the plus 1 server */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_FIFO "/tmp/server-fifo"

char buf[100];                  /* holds path of client fifi */

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

static long
getLong(const char *s)
{
    errno = 0;
    long num = strtol(s, 0, 10);
    if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }
    return num;
}

static void
removeFifo()
{
    unlink(buf);
}

int
main(int argc, char* argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s num\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Create FIFO */

    printf("creating FIFO\n");

    snprintf(buf, sizeof buf, "/tmp/client-fifo-%ld", (long) getpid());
    if (mkfifo(buf, S_IRUSR | S_IWUSR) == -1 && errno != EEXIST)
        errExit("mkfifo");
    errno = 0;

    if (atexit(removeFifo) == -1)
        errExit("atexit");

    /* Send request */

    printf("sending request\n");

    int serverFd = open(SERVER_FIFO, O_WRONLY);
    if (serverFd == -1)
        errExit("open");
    struct request req = {
        .pid = getpid(),
        .num = getLong(argv[1]),
    };
    ssize_t numWritten = write(serverFd, &req, sizeof req);
    if (numWritten == -1)
        errExit("write partially/error");

    if ((size_t) numWritten < sizeof req)
        printf("write partially; numWritten: %zd\n", numWritten);

    if (close(serverFd) == -1)
        errExit("close");

    /* Receive response */

    printf("receiving response\n");

    int clientFd = open(buf, O_RDONLY);
    if (clientFd == -1)
        errExit("open");
    struct response resp;
    ssize_t numRead = read(clientFd, &resp, sizeof resp);
    if (numRead == -1)
        errExit("read");

    if ((size_t) numRead < sizeof resp)
        printf("read partially; numRead: %zd\n", numRead);
    else
        printf("=> %d\n", resp.num);

    if (close(clientFd) == -1)
        errExit("close");
}
