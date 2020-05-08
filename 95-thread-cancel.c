/* 95-thread-cancel.c --- cancel thread */
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

static void*
threadFunc(void* arg __attribute__((unused)))
{
    printf("new thread started\n"); /* maybe cancellation point */

    for (int i = 0; ; i++) {
        printf("loop %d\n", i); /* maybe cancellation point */
        sleep(1);               /* cancellation point */
    }

    /* NOTREACHED */
    return NULL;
}

int
main()
{
    pthread_t thread;
    int status;

    status = pthread_create(&thread, NULL, threadFunc, NULL);
    if (status != 0) errExit(status, "pthread_create");

    sleep(3);
    status = pthread_cancel(thread);
    if (status != 0) errExit(status, "pthread_cancel");

    void* res;
    status = pthread_join(thread, &res);
    if (status != 0) errExit(status, "pthread_join");

    printf("PTHREAD_CANCELED: %p\n", PTHREAD_CANCELED);

    if (res == PTHREAD_CANCELED)
        printf("thread canceled\n");
    else
        printf("thread finished normally\n");
}
