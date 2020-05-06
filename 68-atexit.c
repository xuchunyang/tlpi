/* 68-atexit.c --- atexit */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void
f1()
{
    puts("calling f1()");
}

static void
f2()
{
    puts("calling f2()");
}

static void
f3()
{
    puts("calling f3()");
}


int
main(int argc, char* argv[])
{
    if (argc >= 2 && strcmp(argv[1], "--help") == 0) {
        printf("usage: %s [_exit] [sig]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (atexit(f1) == -1) {
        perror("atexit");
        exit(EXIT_FAILURE);
    }

    if (atexit(f2) == -1) {
        perror("atexit");
        exit(EXIT_FAILURE);
    }

    if (atexit(f3) == -1) {
        perror("atexit");
        exit(EXIT_FAILURE);
    }

    switch (argc) {
    case 1:
        puts("normal exit");
        exit(EXIT_SUCCESS);
    case 2:
        puts("abnormal exit via _exit()");
        _exit(EXIT_SUCCESS);
    case 3:
        puts("abnormal exit via sig");
        raise(SIGINT);          /* 2 + 128 = 130，所以 echo $? 为 130 */
        sleep(3);
    }
}
