#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <limits.h>

int
main()
{
    errno = 0;
    long lim = sysconf(_SC_MQ_PRIO_MAX);
    if (lim == -1) {
        if (errno != 0) {
            perror("sysconf");
            exit(EXIT_FAILURE);
        } else
            printf("_SC_MQ_PRIO_MAX is not supported\n");
    } else
        printf("sysconf(_SC_MQ_PRIO_MAX): %ld\n",
               sysconf(_SC_MQ_PRIO_MAX));

    printf("_POSIX_NAME_MAX: %d\n", _POSIX_NAME_MAX);
    printf("_PC_NAME_MAX: %ld\n", pathconf(".", _PC_NAME_MAX));

    printf("_SC_NGROUPS_MAX: %ld\n", sysconf(_SC_NGROUPS_MAX));

    printf("_SC_ARG_MAX: %ld\n", sysconf(_SC_ARG_MAX));

    // printf("%d\n", LOGIN_NAME_MAX);
    printf("_SC_LOGIN_NAME_MAX: %ld\n", sysconf(_SC_LOGIN_NAME_MAX));

    printf("_SC_OPEN_MAX: %ld\n", sysconf(_SC_OPEN_MAX));

    printf("PATH_MAX: %d\n", PATH_MAX);
    printf("_PC_PATH_MAX: %ld\n", pathconf(".", _PC_PATH_MAX));

    printf("_PC_PIPE_BUF: %ld\n", pathconf(".", _PC_PIPE_BUF));
}
