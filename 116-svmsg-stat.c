/* 116-svmsg-stat.c --- 获取消息队列的状态 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
int
main()
{
    int id = 196609;
    struct msqid_ds ds;
    if (msgctl(id, IPC_STAT, &ds) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("消息数量: %lu\n", ds.msg_qnum);
    printf("数据大小的上限: %lu bytes\n", ds.msg_qbytes);
    printf("最近发送时间: %s", ctime(&ds.msg_stime));
    printf("最近接收时间: %s", ctime(&ds.msg_rtime));
}
