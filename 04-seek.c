/* 04-seek.c --- 4-3 使用 seek */
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

static long
getLong(const char *s)
{
  errno = 0;
  long num = strtol(s, 0, 10);
  if (errno != 0) {
    perror("strtol");
    exit(EXIT_FAILURE);
  }
  return num;
}

int
main(int argc, char* argv[])
{
  if (argc < 3 || strcmp(argv[1], "--help") == 0) {
    printf("usage: %s file {r<length>|R<length>|w<string>|s<offset>}...\n",
           argv[0]);
    exit(EXIT_FAILURE);
  }

  int fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    printf("opening file %s: %s\n", argv[1], strerror(errno));
    exit(EXIT_FAILURE);
  }

  size_t len;
  char* buf;
  ssize_t numRead;
  ssize_t numWritten;
  off_t offset;
  for (int ap = 2; ap < argc; ap++) {
    // printf("%d. '%s'\n", ap, argv[ap]);
    switch (argv[ap][0]) {
    case 'r':
    case 'R':
      len = getLong(&argv[ap][1]);
      // printf("%zu\n", len);
      buf = malloc(len);
      if (!buf) {
        perror("malloc");
        exit(EXIT_FAILURE);
      }
      numRead = read(fd, buf, len);
      if (numRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
      }
      if (numRead == 0) {
        printf("%s: end-of-file\n", argv[ap]);
      } else {
        printf("%s: ", argv[ap]);
        for (int i = 0; i < numRead; i++)
          if (argv[ap][0] == 'r')
            printf("%c", isprint(buf[i]) ? buf[i] : '?');
          else
            printf("%02x ", buf[i]);
        printf("\n");
      }
      break;
    case 'w':
      numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
      if (numWritten == -1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
      printf("%s: wrote %ld bytes\n", argv[ap], numWritten);
      break;
    case 's':
      offset = getLong(&argv[ap][1]);
      if (lseek(fd, offset, SEEK_CUR) == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
      }
      printf("%s: seek succeeded\n", argv[ap]);
      break;
    default:
      printf("Argument must start with [rRws]: %s\n", argv[ap]);
      exit(EXIT_FAILURE);
    }
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
}
