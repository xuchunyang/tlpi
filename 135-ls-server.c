/* 135-ls-server.c --- ls server */
#include <errno.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

static void
die(char* s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

int
main(int argc, char** argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "usage: %s port\n", argv[0]);
    }

    char* parsed;
    int port = strtol(argv[1], &parsed, 10);
    if (parsed[0] != '\0') {
        fprintf(stderr, "can't parse port: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        die("socket");

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind (sockfd, (struct sockaddr *)&addr, sizeof addr) == -1)
        die ("bind");

    if (listen (sockfd, 1) == -1)
        die ("listen");

    printf("Listening on %s port %d\n", inet_ntoa(addr.sin_addr), port);

    while (1) {
        printf("waiting for connection...\n");
        int fd = accept(sockfd, NULL, NULL);
        if (fd == -1)
            die("accept");

        FILE *stream = popen("ls -al", "r");
        if (stream == NULL)
            die("popen ls -al");

        char buf[1024];
        size_t numRead = fread(buf, 1, sizeof buf, stream);
        if (write(fd, buf, numRead) == -1) {
            die("write");
        }
        if (close(fd) == -1)
            die("close");

        printf("connection established, working...\n");
    }
}
