#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <assert.h>

static void
die(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

int
main()
{
    char template[] = "testXXXXXX";
    int fd = mkstemp(template);
    if (fd == -1) die("mkstemp");
    printf("Generated %s\n", template);

    int fd2 = open(template, O_RDWR);
    if (fd2 == -1) die("open");

    assert(write(fd, "helloworld", 10) == 10);
    assert(write(fd2, "HELLO", 5) == 5);
}
