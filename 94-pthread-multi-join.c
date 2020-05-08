/* 94-pthread-multi-join.c --- join 任意 thread */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int unjoined = 0;

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void
errExit(int status, char *message)
{
    printf("%s: %s\n", message, strerror(status));
    exit(EXIT_FAILURE);
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

enum tstate {
    TS_ALIVE,
    TS_TERMINATED,
    TS_JOINED
};

struct thread_info
{
    pthread_t tid;
    int id;
    enum tstate state;
    int sleep_time;
};

static void*
thread_start(void* arg)
{
    struct thread_info* info = arg;
    sleep(info->sleep_time);
    printf("Thread %d about to quit\n", info->id);

    int s = pthread_mutex_lock(&mutex);
    if (s != 0) errExit(s, "pthread_mutex_lock");

    unjoined++;
    info->state = TS_TERMINATED;

    s = pthread_mutex_unlock(&mutex);
    if (s != 0) errExit(s, "pthread_mutex_unlock");

    s = pthread_cond_signal(&cond);
    if (s != 0) errExit(s, "pthread_cond_signal");

    return NULL;
}

int
main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s sec...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int const total_threads = argc - 1;
    struct thread_info* info = malloc(total_threads * sizeof *info);
    if (info == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < total_threads; i++) {
        info[i].id = i+1;
        info[i].state = TS_ALIVE;
        info[i].sleep_time = getLong(argv[i+1]);
        int s = pthread_create(&info[i].tid, NULL, thread_start, &info[i]);
        if (s != 0) errExit(s, "pthread_create");
    }

    // pthread_exit(NULL);
    int num_lived = total_threads;
    for (int i = 0; num_lived > 0; i++) {

        printf("looping %d\n", i);

        int s = pthread_mutex_lock(&mutex);
        if (s != 0) errExit(s, "pthread_mutex_lock");

        while (unjoined == 0) {
            s = pthread_cond_wait(&cond, &mutex);
            if (s != 0) errExit(s, "pthread_cond_wait");
        }

        for (int i = 0; i < total_threads; i++) {
            if (info[i].state == TS_TERMINATED) {
                int s = pthread_join(info[i].tid, NULL);
                if (s != 0) errExit(s, "pthread_join");

                info[i].state = TS_JOINED;
                unjoined--;
                num_lived--;

                printf("reaped thread %d (num_lived=%d)\n", info[i].id, num_lived);
            }
        }

        s = pthread_mutex_unlock(&mutex);
        if (s != 0) errExit(s, "pthread_mutex_unlock");
    }
}
