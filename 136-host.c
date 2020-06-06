/* 136-host.c --- try getaddrinfo(3) to impl host(1) */
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>

/*
$ ./136-host.out baidu.com
ipv4 220.181.38.148
ipv4 39.156.69.79

$ ./136-host.out example.com
ipv4 93.184.216.34
ipv6 2606:2800:220:1:248:1893:25c8:1946
*/

int
main(int argc, char* argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "usage: %s hostname\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    struct addrinfo *result;
    struct addrinfo hint = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_DGRAM, /* 只查询 UDP，不然结果重复 */
        .ai_protocol = 0,
        .ai_flags = AI_ALL | AI_V4MAPPED,
    };
    int error = getaddrinfo(argv[1], NULL, &hint, &result);
    if (error) {
        fprintf(stderr, "%s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }

    for (struct addrinfo *rp = result; rp != NULL; rp = rp->ai_next) {
        // printf("%d, (tcp)%d, (udp)%d\n", rp->ai_socktype, SOCK_STREAM, SOCK_DGRAM);
        if (rp->ai_family == AF_INET) {
            char buf[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &((struct sockaddr_in*) rp->ai_addr)->sin_addr, buf, sizeof buf) == NULL) {
                perror("inet_ntop");
                exit(EXIT_FAILURE);
            }
            printf("ipv4 %s\n", buf);
        } else if (rp->ai_family == AF_INET6) {
            char buf[INET6_ADDRSTRLEN];
            if (inet_ntop(AF_INET6, &((struct sockaddr_in6*) rp->ai_addr)->sin6_addr, buf, sizeof buf) == NULL) {
                perror("inet_ntop");
                exit(EXIT_FAILURE);
            }
            printf("ipv6 %s\n", buf);
        } else {
            fprintf(stderr, "Impossible\n");
            exit(EXIT_FAILURE);
        }
    }
    freeaddrinfo(result);
}
