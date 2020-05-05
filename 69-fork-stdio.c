#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int
main()
{
    printf("Hello, world!\n");
    /* if (fflush(stdout) == EOF) { */
    /*     perror("fflush"); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    write(STDOUT_FILENO, "Ciao\n", 5);


    switch (fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        _exit(EXIT_SUCCESS);
    default:
        exit(EXIT_SUCCESS);
    }
}
