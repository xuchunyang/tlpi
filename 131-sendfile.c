/* 131-sendfile.c --- sendfile(2) & socketpair(2) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

static void
die(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

int
main(int argc, char* argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sv[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1)
        die("socketpair");

    int pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {             /* child */
        if (close(sv[1]) == -1)
            die("close");

        int fd = open(argv[1], O_RDONLY);
        if (fd == -1)
            die("open");

        off_t len;
        if (sendfile(fd, sv[0], SEEK_SET, &len, NULL, 0) == -1)
            die("sendfile");

        if (close(sv[0]) == -1)
            die("close");

        _exit(EXIT_SUCCESS);
    }

    if (close(sv[0]) == -1)
        die("close");

    while (1) {
        char buf[1024];
        ssize_t numRead = read(sv[1], buf, sizeof buf);
        if (numRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (numRead == 0) {
            break;
        }
        write(STDOUT_FILENO, buf, numRead);
    }

    if (close(sv[1]) == -1)
        die("close");
}


