/* 115-svmsg-receive.c --- 从消息队列接收一条消息 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#define MAX_MTEXT 1024

struct themsg {
    long mtype;
    char mtext[MAX_MTEXT];
};

int
main()
{
    int id = 196609;
    struct themsg msg;
    if (msgrcv(id, &msg, MAX_MTEXT, 0, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("从消息队列 %d 收到消息: mtype=%ld, mtext='%s'\n",
           id,
           msg.mtype,
           msg.mtext);
}
