/* 92-dynamic-init-mutex.c --- Create pthread_mutex_t dynamically */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static void
errExit(int status, char *message)
{
    printf("%s: %s\n", message, strerror(status));
    exit(EXIT_FAILURE);
}

static void *
thread_start(void *arg)
{
    int s;
    pthread_mutex_t* mutex = arg;
    printf("locking\n");
    s = pthread_mutex_lock(mutex);
    if (s)
        errExit(s, "pthread_mutex_lock");

    printf("locking 2\n");
    s = pthread_mutex_lock(mutex);
    if (s)
        errExit(s, "pthread_mutex_lock");

    printf("critical section\n");
    pthread_mutex_unlock(mutex);
    return NULL;
}

int
main()
{
    int s;

    pthread_mutex_t* mutex = malloc(sizeof *mutex);
    if (mutex == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    pthread_mutexattr_t mutexAttr;
    s = pthread_mutexattr_init(&mutexAttr);
    if (s)
        errExit(s, "pthread_mutexattr_init");
    s = pthread_mutexattr_settype(&mutexAttr,
                                  /* 检查 mutex 的一些错误用法 */
                                  PTHREAD_MUTEX_ERRORCHECK);
    if (s)
        errExit(s, "pthread_mutexattr_settype");

    s = pthread_mutex_init(mutex, &mutexAttr);
    if (s)
        errExit(s, "pthread_mutex_init");

    pthread_t thread;
    s = pthread_create(&thread, NULL, thread_start, mutex);
    if (s)
        errExit(s, "pthread_create");

    s = pthread_join(thread, NULL);
    if (s)
        errExit(s, "pthread_join");

    s = pthread_mutex_destroy(mutex);
    if (s)
        errExit(s, "pthread_mutex_destroy");
    free(mutex);
}
