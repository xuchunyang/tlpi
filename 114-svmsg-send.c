/* 114-svmsg-send.c --- 给 System V 消息队列发送一条消息 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
struct themsg {
    long mtype;
    char mtext[1024];
};

int
main()
{
    struct themsg msg = {
        .mtype = 100,
        .mtext = "hello",
    };

    int id = 196609;
    if (msgsnd(id, &msg, strlen(msg.mtext)+1, 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Message sent to %d (mtype=%ld, mtext='%s')\n",
           id,
           msg.mtype,
           msg.mtext);
}
