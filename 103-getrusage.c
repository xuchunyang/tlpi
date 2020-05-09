/* 103-getrusage.c --- getrusage(2) */
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

int
main()
{
  // sleep(1);
  long sum = 0;
  for (int i = 0; i < 100000000; i++)
    sum += i * i;
  printf("%ld\n", sum);

  char* buf = malloc(1024 * 1024 * 10);
  if (buf == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  struct rusage ru;
  if (getrusage(RUSAGE_SELF, &ru) == -1) {
    perror("getrusage");
    exit(EXIT_FAILURE);
  }

  printf("%ld.%09ld\n", (long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec);
  printf("%ld.%09ld\n", (long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec);
}
