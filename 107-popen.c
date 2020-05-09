/* 107-popen.c --- popen(3) pipe to subprocess */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void
upcase(char *s)
{
    FILE* stream = popen("tr a-z A-Z", "w");
    if (stream == NULL) {
        perror("popen tr");
        exit(EXIT_FAILURE);
    }

    size_t len = strlen(s);
    size_t numWritten;
    if ((numWritten = fwrite(s, len, 1, stream)) != 1) {
        printf("%zu, %zu\n", len, numWritten);
        perror("fwrite");
        exit(EXIT_FAILURE);
    }

    if (pclose(stream) == -1) {
        perror("pclose");
        exit(EXIT_FAILURE);
    }
}

static void
date()
{
    FILE* stream = popen("date", "r");
    if (stream == NULL) {
        perror("popen date");
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    size_t numRead = fread(buf, 1, sizeof buf, stream);

    // setbuf(stdout, NULL);       /* Disable stdout buffer */

    printf("\n'");
    fflush(stdout);
    write(STDOUT_FILENO, buf, numRead-1);
    printf("'");
    // printf("\n'%s'\n", buf);

    if (pclose(stream) == -1) {
        perror("pclose");
        exit(EXIT_FAILURE);
    }
}

int
main()
{
    upcase("hello, world");     /* write */
    date();                     /* read */
    /* TODO read & write, BSD's popen supports r+ */
}
