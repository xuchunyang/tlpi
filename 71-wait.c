/* 71-wait.c --- wait process */
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int
main()
{
  printf("If no child process, wait(0) should return -1? %s\n",
         wait(0) == -1 ? "Y" : "N");
  printf("errno == ECHILD? %s\n",
         errno == ECHILD ? "Y" : "N");
  perror("wait");


  pid_t childPid;

  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);

  case 0:
    printf("child id=%ld sleep for 3s...\n",
           (long)getpid());
    sleep(3);
    printf("child id=%ld about to quit\n",
           (long)getpid());
    _exit(EXIT_SUCCESS);

  default:
    childPid = wait(0);
    if (childPid == -1) {
      perror("wait");
      exit(EXIT_FAILURE);
    }
    printf("child process id=%ld quited\n",
           (long) childPid);
  }
}
