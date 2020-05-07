/* 86-pthread-id.c --- pthread_equal */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define NUM_THREADS 5

struct thread_info {
    pthread_t thread;
    int id;
};

static void*
thread_start(void *arg)
{
    struct thread_info* info =  arg;
    printf("%s\n",
           pthread_equal(info->thread, pthread_self()) ? "Y" : "N");

    printf("Thread %d\n", info->id);


    return NULL;
}

int
main()
{
    struct thread_info tinfo[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        tinfo[i].id = i;
        int status = pthread_create(&tinfo[i].thread, NULL, thread_start, &tinfo[i]);
        if (status != 0) {
            printf("pthread_create: %s\n", strerror(status));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int status = pthread_join(tinfo[i].thread, NULL);
        if (status != 0) {
            printf("pthread_join: %s\n", strerror(status));
            exit(EXIT_FAILURE);
        }
        printf("Joined with thread %d\n", tinfo[i].id);
    }
}
