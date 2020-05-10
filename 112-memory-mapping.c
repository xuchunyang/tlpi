/* 112-memory-mapping.c --- use mmap(2) to read a file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

int
main(int argc, char* argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Page Size: 4096 bytes
    // printf("Page Size: %ld bytes\n", sysconf(_SC_PAGESIZE));

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    char* addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size) {
        printf("partial/failed write");
    }
}
