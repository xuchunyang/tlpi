/* 89-pthread-exit.c --- ex29-2. calling pthread_exit in main() */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void*
thread_start(void* arg)
{
    printf("thread is running\n");
    sleep(10);
    printf("thread is about to finish\n");
    return arg;
}

int
main()
{
    pthread_t th;
    int s = pthread_create(&th, NULL, thread_start, NULL);
    if (s) {
        printf("%s\n", strerror(s));
        exit(EXIT_FAILURE);
    }

    printf("running pthread_exit in main...\n");
    /* To allow other threads to continue execution, the main thread should */
    /* terminate by calling pthread_exit() rather than exit(3).  */
    pthread_exit(NULL);
    printf("pthread_exit done\n");
}
