/* 134-file-listing-server.c --- directory listing HTTP 服务器 */

/* 有时没问题，其它时候报错 */
/* ~ $ curl http://localhost:8888/ */
/* curl: (56) Recv failure: Connection reset by peer */

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

__attribute__((unused))
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

static void
Write(int fd, const char* s)
{
    size_t len = strlen(s);
    ssize_t numWritten = write(fd, s, len);
    if (numWritten == -1)
        die("write");
    if (numWritten != (ssize_t)len) {
        fprintf(stderr, "partial write\n");
        exit(EXIT_FAILURE);
    }
}

static char*
build_header(int content_length)
{
    static char buffer[1024];
    snprintf(buffer,
             sizeof buffer,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/plain; charset=UTF-8\r\n"
             "Content-Length: %d\r\n"
             "\r\n",
             content_length);
    return buffer;
}

static void
serve(int fd)
{
    DIR* dirp = opendir(".");
    if (dirp == NULL) {
        Write(fd, strerror(errno));
        die("opendir");
    } else {
        char buffer[10240];
        int len = 0;
        while(1) {
            struct dirent *dp = readdir(dirp);
            if (dp == NULL)
                break;

            memcpy(buffer + len, dp->d_name, dp->d_namlen);
            len += dp->d_namlen;
            buffer[len] = '\n';
            len++;
        }
        closedir(dirp);

        printf("len = %d\n", len);

        char* header = build_header(len);
        Write(fd, header);
        ssize_t numWritten = write(fd, buffer, len);
        if (numWritten == -1)
            die("write to fd");
        else if (numWritten != len) {
            fprintf(stderr, "partial write, writen %d, total %d\n", numWritten, len);
            exit(EXIT_FAILURE);
        } else
            fprintf(stderr, "ok write, writen %d, total %d\n", numWritten, len);
    }

    /* char c; */
    /* read(fd, &c, 1); */
    if (close(fd) == -1)
        die("close fd");
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

        serve(fd);
        printf("connection established, working...\n");
    }
}
