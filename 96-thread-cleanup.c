/* 96-thread-cleanup.c --- pthread_cleanup_push */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void
errExit(int status, char *message)
{
    printf("%s: %s\n", message, strerror(status));
    exit(EXIT_FAILURE);
}

static void
cleanup(void* arg)
{
    printf("thread cleanup\n");

    free(arg);
}

static void*
thread_start(void* arg __attribute__((unused)))
{
    printf("new thread started\n");

    char *s = malloc(1024);
    if (s == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    pthread_cleanup_push(cleanup, s);

    printf("thread do some work\n");

    // return NULL;
    pthread_exit(NULL);
    printf("not run\n");

    pthread_cleanup_pop(1);
    return NULL;
}

int
main()
{
    pthread_t thread;
    int status;

    status = pthread_create(&thread, NULL, thread_start, NULL);
    if (status != 0) errExit(status, "pthread_create");

    pthread_exit(NULL);
}
