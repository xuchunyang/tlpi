/* 93-condition-variable.c --- 条件变量 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int avail = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void
errExit(int status, char *message)
{
    printf("%s: %s\n", message, strerror(status));
    exit(EXIT_FAILURE);
}

static void*
thread_start(void* arg)
{
    /* printf("生产中...") */
    /* sleep(1); */

    int s;
    s = pthread_mutex_lock(&mutex);
    if (s != 0) errExit(s, "pthread_mutex_lock");

    avail++;
    printf("producing %d\n", avail);
    if (avail == 3) sleep(1);

    s = pthread_mutex_unlock(&mutex);
    if (s != 0) errExit(s, "pthread_mutex_unlock");

    s = pthread_cond_signal(&cond);
    if (s != 0) errExit(s, "pthread_cond_signal");

    return arg;
}

int
main()
{
    int s;
    pthread_t th1, th2, th3;

    s = pthread_create(&th1, NULL, thread_start, NULL);
    if (s != 0)
        errExit(s, "pthread_create 1");

    s = pthread_create(&th2, NULL, thread_start, NULL);
    if (s != 0)
        errExit(s, "pthread_create 2");

    s = pthread_create(&th3, NULL, thread_start, NULL);
    if (s != 0)
        errExit(s, "pthread_create 3");

    for(int i = 0;;i++) {
        printf("loop %d...\n", i);

        s = pthread_mutex_lock(&mutex);
        if (s != 0)
            errExit(s, "pthread_mutex_lock");

        while (avail == 0) {    /* 判断条件 */
            s = pthread_cond_wait(&cond, &mutex);
            if (s != 0)
                errExit(s, "pthread_cond_wait");
        }

        while (avail > 0) {
            printf("consuming %d\n", avail);
            avail--;
        }

        s = pthread_mutex_unlock(&mutex);
        if (s != 0)
            errExit(s, "pthread_mutex_unlock");
    }

    /*
      for(int i = 0;;i++) {
      printf("checking %d...\n", i);

      s = pthread_mutex_lock(&mutex);
      if (s != 0) errExit(s, "pthread_mutex_lock");

      while (avail > 0) {
      printf("avail: %d\n", avail);
      avail--;
      }

      s = pthread_mutex_unlock(&mutex);
      if (s != 0) errExit(s, "pthread_mutex_unlock");
      }
    */
}
