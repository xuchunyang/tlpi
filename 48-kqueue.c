/* 48-kqueue.c --- FreeBSD kqueue */

#ifdef __APPLE__

#include <unistd.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char* argv[])
{
    int fd = kqueue();
    if (fd == -1) {
        perror("kqueue");
        exit(EXIT_FAILURE);
    }

    struct kevent ke = {
        .filter = EVFILT_VNODE,
        .flags = EV_ENABLE,
    };
    struct kevent k;
    if (kevent(fd, &ke, 1, &k, 1, 0) == -1) {
        printf("kevent errors\n");
        exit(EXIT_FAILURE);
    }

/* TODO */

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

#else

int main(){}

#endif
