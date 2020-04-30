/* 26-putenv.c --- ex6-3. putenv */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern char** environ;

/* NOTE 不会检查多次定义 */
// int unsetenv(const char *)
int
myunsetenv(const char* name)
{
    size_t size = strlen(name) + 2;
    char* buf = malloc(size);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    snprintf(buf, size, "%s=", name);

    bool found = false;
    for (char** ep = environ; *ep; ep++) {
        if (strncmp(*ep, buf, size-1) == 0)
            found = true;
        if (found)
            *ep = *(ep+1);
    }

    free(buf);
    return 0;
}

// int setenv(const char *__name, const char *__value, int __overwrite)
int
mysetenv(const char* name, const char* value, int overwrite)
{
    if (getenv(name)) {
        if (overwrite) {
            // TODO
            return 0;
        } else
            /* no-op */
            return 0;
    } else {
        size_t buf_size = strlen(name) + 1 + strlen(value) + 1;
        char* buf = malloc(buf_size);
        if (buf == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        snprintf(buf, buf_size, "%s=%s", name, value);
        return putenv(buf);
    }
}

int
main()
{
    *environ = 0;
    if (mysetenv("MSG", "hello", 0) == -1) {
        perror("mysetenv");
        exit(EXIT_FAILURE);
    }
    if (mysetenv("MSG", "HELLO", 1) == -1) {
        perror("mysetenv");
        exit(EXIT_FAILURE);
    }
    if (mysetenv("SHELL", "bash", 1) == -1) {
        perror("mysetenv");
        exit(EXIT_FAILURE);
    }
    for (char** ep = environ; *ep; ep++)
        puts(*ep);

    puts("----");

    myunsetenv("MSG");
    for (char** ep = environ; *ep; ep++)
        puts(*ep);
}
