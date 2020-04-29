/* 05-tee.c --- ex4-1 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <getopt.h>
#include <sys/stat.h>

#define BUF_SIZE 1024

int
main(int argc, char* argv[])
{
  bool append = false;
  int opt;
  while ((opt = getopt(argc, argv, "a")) != -1) {
    if (opt == '?') {
      printf("usage: %s [-a] file\n", argv[0]);
      exit(EXIT_FAILURE);
    }
    if (opt == 'a')
      append = true;
  }

  if (argv[optind] == NULL) {
    printf("usage: %s [-a] file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* IDEA handle zero or more-than-one files */

  int fd = open(argv[optind],
                O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC),
                S_IRUSR | S_IWUSR);
  if (fd == -1) {
    printf("opening file %s: %s\n", argv[optind], strerror(errno));
    exit(EXIT_FAILURE);
  }

  char buf[BUF_SIZE];
  ssize_t numRead;
  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
    if (write(STDOUT_FILENO, buf, numRead) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    if (write(fd, buf, numRead) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }
  if (numRead == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
}
