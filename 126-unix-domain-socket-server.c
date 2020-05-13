/* 126-unix-domain-socket-server.c --- Unix Domain Socket (the server) */
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <assert.h>

#define SOCKET_FILE "/tmp/socket"

static void
cleanup()
{
    printf("cleanup - deleting %s\n", SOCKET_FILE);
    if (unlink(SOCKET_FILE) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
}

static void
handler(int sig __attribute__((unused)))
{
    cleanup();

    assert(signal(SIGINT, SIG_DFL) != SIG_ERR);

    assert(raise(SIGINT) != -1);
}

static void
work(int fd)
{
    while(1) {
        char buf[1024];
        ssize_t numRead = read(fd, buf, sizeof buf);
        if (numRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (numRead == 0) {
            if (fd > STDERR_FILENO) {
                printf("Closing connection\n");
                if (close(fd) == -1) {
                    perror("close");
                    exit(EXIT_FAILURE);
                }
            }
            break;
        }
        ssize_t numWritten = write(STDOUT_FILENO, buf, numRead);
        if (numRead == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        if (numWritten != numRead) {
            printf("ERROR: partial written\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < numRead; i++)
            if (islower(buf[i]))
                buf[i] = toupper(buf[i]);
        if (write(fd, buf, numRead) != numRead) {
            printf("partial/error write");
            exit(EXIT_FAILURE);
        }
    }
}

int
main()
{
    int unix_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (unix_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr = {
        .sun_family = AF_UNIX,
        .sun_path = SOCKET_FILE,
    };
    if (bind(unix_socket, (struct sockaddr*) &addr, sizeof addr) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (atexit(cleanup) == -1) {
        perror("atexit");
        exit(EXIT_FAILURE);
    }

    assert(signal(SIGINT, handler) != SIG_ERR);

    if (listen(unix_socket, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1) {
        int fd = accept(unix_socket, NULL, NULL);
        if (fd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        time_t t = time(NULL);
        struct tm* tt = localtime(&t);
        char tb[100];
        if (strftime(tb, sizeof tb, "%T", tt) == 0) {
            perror("strftime");
            exit(EXIT_FAILURE);
        }
        printf("[%s] Accept connection\n", tb);

        work(fd);
    }
}
