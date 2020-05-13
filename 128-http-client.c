/* 128-http-client.c --- basic http client */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
int
main()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // python3 -m http.server
    struct in_addr iaddr;
    /* if (inet_aton("localhost", &iaddr) != 0) { */
    /*     printf("inet_aton error\n"); */
    /*     exit(EXIT_FAILURE); */
    /* } */
    assert(inet_pton(AF_INET, "0.0.0.0", &iaddr) == 1);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = iaddr,
        .sin_port = htons(8000),
    };

    /* struct sockaddr_in addr = {0}; */
    /* memset(&addr, 0, sizeof addr); */
    /* addr.sin_family = AF_INET; */
    /* addr.sin_port = htons(x) */
    printf("%s\n", inet_ntoa(addr.sin_addr));

    if (connect(sfd, (struct sockaddr*)&addr, sizeof addr) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    snprintf(buf, sizeof buf, "GET / HTTP/1.1\r\nHost: localhost:8000\r\n\r\n");
    ssize_t numWritten = write(sfd, buf, strlen(buf));
    printf("Wrote %zd bytes\n", numWritten);

    while (1) {
        ssize_t numRead = read(sfd, buf, sizeof buf);
        if (numRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (numRead == 0) {     /* eof? */
            break;
        }
        write(STDOUT_FILENO, buf, numRead);
        // printf("%s", buf);
        // printf("Read %zd bytes, content: '%s'\n", numRead, buf);
    }
}
