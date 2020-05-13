/* 127-unix-domain-socket-client.c --- the client */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

int
main(int argc, char* argv[])
{
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s path/to/unix/domain/socket message\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr = {
        .sun_family = AF_UNIX,
        // .sun_path = "/tmp/socket", /* argv[1] */
    };
    // strcpy(addr.sun_path, argv[1]);
    strncpy(addr.sun_path, argv[1], sizeof addr.sun_path);

    if (connect(sfd, (struct sockaddr*)&addr, sizeof addr) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    ssize_t numWritten = write(sfd, argv[2], strlen(argv[2]));

    printf("Wrote %zd bytes\n", numWritten);

    char buf[1024];
    ssize_t numRead = read(sfd, buf, sizeof buf);

    printf("Read %zd bytes, content: '%s'\n", numRead, buf);
}
