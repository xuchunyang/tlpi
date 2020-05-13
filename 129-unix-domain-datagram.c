/* 129-unix-domain-datagram.c --- Unix Domain Socket 数据包 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#define MY_SOCKET_PATH "/tmp/socket"
#define BUF_SIZE 10

static void
die(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

static void
server()
{
    if (remove(MY_SOCKET_PATH) == -1 && errno != ENOENT)
        die("remove");

    int sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1)
        die("socket");

    struct sockaddr_un addr = {
        .sun_family = AF_UNIX,
        .sun_path   = MY_SOCKET_PATH,
    };
    if (bind(sfd, (struct sockaddr*) &addr, sizeof addr) == -1)
        die("bind");

    printf("Serving at %s\n", addr.sun_path);

    while(1) {
        char buf[BUF_SIZE];
        ssize_t numBytes = recvfrom(sfd, buf, sizeof buf, 0, NULL, NULL);
        if (numBytes == -1)
            die("recvfrom");

        printf("Server received (%ld bytes)\n", numBytes);

        /* NOTE 服务器要想给客户端发送数据，那么要求客户端也 bind 才行 */
    }
}

static void
client(char *s)
{
    int sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1)
        die("socket");

    struct sockaddr_un addr = {
        .sun_family = AF_UNIX,
        .sun_path   = MY_SOCKET_PATH,
    };

    ssize_t numBytes = sendto(sfd, s, strlen(s), 0, (struct sockaddr*) &addr, sizeof addr);
    if (numBytes == -1)
        die("sendto");
    printf("Sent %ld bytes to server\n", numBytes);
}

int
main(int argc, char* argv[])
{
    if (argc == 1)
        server();
    else
        client(argv[1]);
}
