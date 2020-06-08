/* 139-my-private-ip.c --- ifconfig 的地址 */
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

static void
must(int error)
{
    if (error) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }
}

int
main(void)
{
    struct addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo* result;
    must(getaddrinfo("example.com", "http", &hints, &result));

    struct addrinfo* rp;
    for (rp = result; rp; rp = rp->ai_next) {
        int sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == -1) {
            perror("connect");
            exit(EXIT_FAILURE);
        }

        struct sockaddr addr = {0};
        must(getsockname(sfd, &addr, &rp->ai_addrlen));

        char host[100];
        char service[100];
        must(getnameinfo(&addr,
                         rp->ai_addrlen, host, sizeof host, service, sizeof service, 0));
        /* 外联的链接，可以获得本地 IP */
        // '192.168.2.102', '60411'
        printf("'%s', '%s'\n", host, service);
        close(sfd);
    }
}
