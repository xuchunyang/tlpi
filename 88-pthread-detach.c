/* 88-pthread-detach.c --- joinable vs detch  */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void*
thread_start(void *arg)
{
    return arg;
}

static void
errExit(int status, char const* s)
{
    printf("%s: %s\n", s, strerror(status));
    exit(EXIT_FAILURE);
}

int
main()
{
    pthread_t thread;
    int status = pthread_create(&thread, NULL, thread_start, NULL);
    if (status != 0) errExit(status, "pthread_create");

    status = pthread_detach(thread);
    if (status != 0) errExit(status, "pthread_detach");

    status = pthread_join(thread, NULL);
    if (status != 0) {
        if (status == EINVAL) {
            printf("pthread_join: thread is not joinable\n");
            exit(EXIT_FAILURE);
        } else
            errExit(status, "pthread_join");
    }
}
