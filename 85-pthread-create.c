/* 85-pthread-create.c --- pthread-create(3) */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void *
thread_start(void *arg)
{
    char *msg = arg;
    printf("=> %s\n", msg);
    if (strcmp(msg, "sleep") == 0)
        sleep(3);
    // pthread_exit(NULL);
    // return NULL;
    int len = strlen(msg);
    return (void*) len;
}

static void
errExit(int status, char *message)
{
    printf("%s: %s\n", message, strerror(status));
    exit(EXIT_FAILURE);
}

int
main(int argc, char* argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s msg\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_t thread;
    int s;

    s = pthread_create(&thread, NULL, thread_start, argv[1]);
    if (s != 0) errExit(s, "pthread_create");

    void *res = NULL;

    s = pthread_join(thread, &res);
    if (s != 0) errExit(s, "pthread_join");

    printf("thread value: %p\n", res);
}
