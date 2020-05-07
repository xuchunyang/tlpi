/* 87-pthread-join.c --- pthread_join */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void*
thread_start(void* arg)
{
    return arg;
}

int
main()
{
    pthread_t thread;
    int status = pthread_create(&thread, NULL, thread_start, NULL);
    if (status != 0) {
        printf("pthread_create: %s\n", strerror(status));
        exit(EXIT_FAILURE);
    }

    status = pthread_join(thread, NULL);
    if (status != 0) {
        printf("pthread_join 1: %s\n", strerror(status));
        exit(EXIT_FAILURE);
    }

    status = pthread_join(thread, NULL);
    if (status != 0) {
        if (status == ESRCH)
            printf("No such thread\n");
        else
            printf("pthread_join 2: %s\n", strerror(status));
        exit(EXIT_FAILURE);
    }
}
