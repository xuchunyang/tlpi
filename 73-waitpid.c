/* 73-waitpid.c --- waitpid */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    pid_t w;
    int wstatus;
    pid_t childPid = fork();
    int code;
    switch (childPid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0:
        printf("Child PID is %ld\n", (long) getpid());
        if (argc == 1)
            pause();
        errno = 0;
        code = strtol(argv[1], 0, 10);
        if (errno) {
            perror("strtol");
            printf("argv[1] = '%s'\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        printf("child about to quit with _exit(%d)\n", code);
        _exit(code);

    default:
        do {
            w = waitpid(childPid, &wstatus, WUNTRACED | WCONTINUED);
            if (w == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(wstatus)) {
                printf("exited, status=%d\n", WEXITSTATUS(wstatus));
            } else if (WIFSIGNALED(wstatus)) {
                printf("killed by signal %d\n", WTERMSIG(wstatus));
            } else if (WIFSTOPPED(wstatus)) {
                printf("stopped by signal %d\n", WSTOPSIG(wstatus));
            } else if (WIFCONTINUED(wstatus)) {
/* NOTE macos 下没效果
   19    SIGCONT      discard signal       continue after stop

linux 下有效，因为 SIGCONT 的 default action 不同
SIGCONT      P1990      Cont    Continue if stopped
*/
                printf("continued\n");
            }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
        exit(EXIT_SUCCESS);

        sleep(1);
    }

}
