/* 108-mkfifo.c --- create FIFO (named pipe) file with mkfifo(2) */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int
main()
{
    if (mkfifo("fifo", S_IRUSR | S_IWUSR) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
}
