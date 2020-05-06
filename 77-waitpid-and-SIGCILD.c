/* 77-waitpid-and-SIGCILD.c --- waitpid and SIGCILD */
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_CHILDREN 3

static volatile int numLiveChildren = 0;

static void
handler(int sig)
{
    int savedErrno = errno;

    psignal(sig, NULL);
    pid_t childPid;
    while ((childPid = waitpid(-1, NULL, WNOHANG)) > 0) {
        printf("%ld quited\n", (long) childPid);
        numLiveChildren--;
    }
    sleep(5);

    errno = savedErrno;
}

int
main()
{
    setbuf(stdout, NULL);

    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1) {
        perror("sigprocmask");
    }

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    numLiveChildren = NUM_CHILDREN;
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t childPid = fork();
        if (childPid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (childPid == 0) {
            printf("%ld sleep for %ds\n", (long) getpid(), i);
            sleep(i);
            _exit(EXIT_SUCCESS);
        }
    }

    sigset_t emptyMask;
    sigemptyset(&emptyMask);

    int sigCnt = 0;
    while (numLiveChildren > 0) {
        if (!(sigsuspend(&emptyMask) == -1 && errno == EINTR)) {
            printf("sigsuspend: impossible to reach here");
            exit(EXIT_FAILURE);
        }
        sigCnt++;
    }

    printf("All child processes quited, SIGCHLD signal was caught %d times\n",
           sigCnt);
}
