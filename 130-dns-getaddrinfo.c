/* 130-dns-getaddrinfo.c --- DNS 查询 */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
  $ host example.com
  example.com has address 93.184.216.34
  example.com has IPv6 address 2606:2800:220:1:248:1893:25c8:1946
  example.com mail is handled by 0 .
*/
int
main()
{
    struct addrinfo *res0;
    struct addrinfo hint = {
        .ai_family = AF_UNSPEC,
        .ai_flags = AI_ALL | AI_CANONNAME,
    };
    int status = getaddrinfo("example.com",
                             NULL,
                             &hint,
                             &res0);
    if (status != 0) {
        printf("getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    for (struct addrinfo *res = res0; res; res = res->ai_next) {
        // printf("ai_canonname: %s\n", res->ai_canonname);
        // res->ai_family
        /* XXX not work as expected */
        char buf[100];
        puts(inet_ntop(res->ai_family, res->ai_addr, buf, res->ai_addrlen));
    }

    freeaddrinfo(res0);
}
