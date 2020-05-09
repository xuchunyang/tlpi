/* 108-mkfifo.c --- create FIFO (named pipe) file with mkfifo(2) */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int
main()
{
    if (mkfifo("fifo", S_IRUSR | S_IWUSR) == -1) {
        if (errno == EEXIST) {
            printf("fifo exists\n");
            exit(EXIT_SUCCESS);
        }
        else {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("fifo created\n");
        exit(EXIT_SUCCESS);
    }
}
