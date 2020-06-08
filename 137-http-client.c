/* 137-http-client.c --- basic http client */
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_REQ_SIZE 1024

int
main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "usage: %s hostname", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *hostname = argv[1];
    const struct addrinfo hints = {
        .ai_family = AF_UNSPEC, /* Allows IPv4 or IPv6 */
        .ai_socktype = SOCK_STREAM, /* HTTP uses TCP */
    };

    struct addrinfo *result;
    int status = getaddrinfo(hostname, "http", &hints, &result);
    if (status != 0) {
        fprintf(stderr, "%s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    struct addrinfo *rp;
    int sfd;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        switch (rp->ai_socktype) {
        case SOCK_STREAM:
            printf("TCP\n");
            break;
        case SOCK_DGRAM:
            printf("UDP\n");
            break;
        case SOCK_RAW:
            printf("RAW\n");
            break;
        }

        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            perror("socket");
            continue;           /* On error, try next address */
        }

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;              /* Success */

        /* Conect failed: close this socket and try next address */

        close(sfd);
    }

    freeaddrinfo(result);

    if (rp == NULL) {
        fprintf(stderr, "can't connect to %s\n", hostname);
        exit(EXIT_FAILURE);
    }

    char req_buf[MAX_REQ_SIZE];
    snprintf(req_buf, sizeof req_buf,
             ("GET / HTTP/1.1\r\n"
              "Host: %s\r\n"
              "Connection: close\r\n" /* since we don't detect Content-Length */
              "\r\n"), hostname);
    ssize_t numWritten = write(sfd, req_buf, strlen(req_buf));
    if (numWritten == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    } else if ((size_t) numWritten < strlen(req_buf)) {
        if (numWritten == 0)
            perror("write");
        else
            fprintf(stderr, "partial write, %zd/%ld", numWritten, strlen(req_buf));
        exit(EXIT_FAILURE);
        
    }

    char res_buf[10240];
    for (int i = 0; ; i++) {
        ssize_t numRead = read(sfd, res_buf, sizeof res_buf);
        if (numRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (numRead == 0) {     /* eof? */
            break;
        }
        write(STDOUT_FILENO, res_buf, numRead);
    }
}
