/* 90-race-condition.c --- multiple threads race condition */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
static int global = 0;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *
thread_start(void *arg)
{
    int loop = *((int*)arg);
    for (int i = 0; i < loop; i++) {
        pthread_mutex_lock(&mtx);
        int local = global;
        local += 1;
        global = local;
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

static long
getLong(const char *s)
{
    errno = 0;
    long num = strtol(s, 0, 10);
    if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }
    return num;
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
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("%s: [loop]\n", argv[0]);
    }

    int loop = argc > 1 ? getLong(argv[1]) : 1000;

    int status;

    pthread_t t1;
    status = pthread_create(&t1, NULL, thread_start, &loop);
    if (status != 0) errExit(status, "pthread_create");

    pthread_t t2;
    status = pthread_create(&t2, NULL, thread_start, &loop);
    if (status != 0) errExit(status, "pthread_create");

    status = pthread_join(t1, NULL);
    if (status != 0) errExit(status, "pthread_join");

    pthread_join(t2, NULL);
    if (status != 0) errExit(status, "pthread_join");

    printf("Get  %d\nwant %d\ndiff %d\n",
           global,
           2 * loop,
           2 * loop - global
        );
}
