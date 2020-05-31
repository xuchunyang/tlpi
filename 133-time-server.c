/* 133-time-server.c --- 返回时间的 HTTP 服务器 */
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

static char *
now(char *fmt)
{
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    if (tm == NULL)
        return NULL;

    static char buffer[1024];
    size_t size = strftime(buffer, sizeof buffer, fmt, tm);
    if (size == 0)
        return NULL;
    return buffer;
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

    if (listen (sockfd, 5) == -1)
        die ("listen");

    printf("Listening on %s port %d\n", inet_ntoa(addr.sin_addr), port);

    while (1) {
        printf("waiting for connection...\n");
        int fd = accept(sockfd, NULL, NULL);
        if (fd == -1)
            die("accept");
        printf("connection established, working...\n");
        char* response = now(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; charset=UTF-8\r\n"
            "Content-Length: 26\r\n"
            "\r\n"
            "%Y-%m-%d %H:%M:%S %z\n");
        size_t len = strlen(response);
        ssize_t numWritten = write(fd, response, len);
        if (numWritten == -1)
            die("write");
        if (numWritten != (ssize_t)len) {
            fprintf(stderr, "partial write\n");
            exit(EXIT_FAILURE);
        }
        if (close(fd) == -1)
            die("close fd");
    }
}
