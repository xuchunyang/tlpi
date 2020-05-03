#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s dirname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("mkdir '%s'...\n", argv[1]);
    if (mkdir(argv[1],
              S_IRWXU
              // S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
              // 0700
            ) == -1) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    } else
        printf("directory created\n");

    char buf[1024];
    snprintf(buf, sizeof buf, "file %s", argv[1]);
    printf("running '%s'...\n", buf);
    system(buf);

    if (rmdir(argv[1]) == -1) {
        perror("rmdir");
        exit(EXIT_FAILURE);
    }
}
