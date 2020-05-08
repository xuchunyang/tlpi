/* 91-mutex-dead-lock.c --- dead lock of two mutexe */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

static void *
thread1_start(void *arg __attribute__((unused)))
{
    pthread_mutex_lock(&mutex1);
    printf("mutex1 locked\n");
    sleep(1);
    printf("wait for mutex2\n");
    pthread_mutex_lock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

static void *
thread2_start(void *arg __attribute__((unused)))
{
    pthread_mutex_lock(&mutex2);
    printf("mutex2 locked\n");
    sleep(2);
    printf("wait for mutex1\n");
    pthread_mutex_lock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

static void
errExit(int status, char *message)
{
    printf("%s: %s\n", message, strerror(status));
    exit(EXIT_FAILURE);
}

int
main()
{
    pthread_t t1, t2;
    int status;

    status = pthread_create(&t1, NULL, thread1_start, NULL);
    if (status != 0)
        errExit(status, "pthread_create");

    status = pthread_create(&t2, NULL, thread2_start, NULL);
    if (status != 0)
        errExit(status, "pthread_create");

    pthread_exit(NULL);
}
