/* 27-user.c --- Get current user */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <errno.h>

int
main()
{
    uid_t uid = getuid();
    printf("real user ID,      getuid() = %d\n", uid);

    uid_t euid = geteuid();
    printf("effective user ID, geteuid() = %d\n", euid);

    errno = 0;
    struct passwd* pwd = getpwuid(uid);
    if (pwd == NULL) {
        if (errno != 0) {
            printf("No such uid: %d\n", uid);
        } else {
            perror("getpwuid");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("username: %s\n", pwd->pw_name);
        printf("home:     %s\n", pwd->pw_dir);
        printf("shell:    %s\n", pwd->pw_shell);
    }
}
