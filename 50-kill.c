/* 50-kill.c --- Send signal to process */

#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <strings.h>

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

int
main(int argc, char* argv[])
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s pid signal_name|signal_id\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    assert(strtol("24", 0, 10) == 24);
    assert(errno == 0);

    pid_t pid = getLong(argv[1]);

    int sig = 0;
    for (size_t s = 1;
         s < sizeof sys_signame / sizeof *sys_signame;
         s++)
        if (strcasecmp(argv[2], sys_signame[s]) == 0) {
            sig = s;
            break;
        }

    if (sig == 0) {
        sig = getLong(argv[2]);
    }

    printf("Running 'kill -%d %d'\n", sig, pid);

    if (kill(pid, sig) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }
}
