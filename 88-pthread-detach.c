/* 88-pthread-detach.c --- ex29-1 joinable vs detch  */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static void*
thread_start(void *arg)
{
    int status = pthread_join(pthread_self(), NULL);
    switch (status) {
    case 0:
      break;

    case EINVAL:
      printf("pthread_join: thread is not joinable\n");
      break;

    default:
      printf("pthread_join: %s\n", strerror(status));
      break;
    }
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

    /* status = pthread_detach(thread); */
    /* if (status != 0) errExit(status, "pthread_detach"); */

    sleep(1);
    status = pthread_join(thread, NULL);
    if (status != 0) {
        if (status == EINVAL) {
            printf("pthread_join: thread is not joinable\n");
            exit(EXIT_FAILURE);
        } else
            errExit(status, "pthread_join");
    }
}
