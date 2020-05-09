/* 97-pthread-once.c --- pthread_once */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

static void
setup()
{
    printf("setup run only once no matter how many time it is called\n");
}

static void*
thread_start(void* arg __attribute__((unused)))
{
    printf("thread_start\n");
    pthread_once(&once, setup);
    return NULL;
}

int
main()
{
    for (int i = 0; i < 10; i++) {
        pthread_t thread;
        int status = pthread_create(&thread, NULL, thread_start, NULL);
        if (status != 0) {
            printf("pthread_create: %s\n", strerror(status));
            exit(EXIT_FAILURE);
        }
    }

    pthread_exit(NULL);
}
