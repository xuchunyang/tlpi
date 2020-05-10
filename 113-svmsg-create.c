/* 113-svmsg-create.c --- 创建一个 System V 消息队列 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h> // S_IRUSR

int
main(int argc, char* argv[])
{
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        printf("usage: %s [delete]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    key_t key = ftok("/tmp/myfile", 'x');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    /* Open or Create a Message Queue */

    int id = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("New or existing IPC ID: %d\n", id);

    /* Delete the Message Queue */

    if (argc > 1) {
        printf("Delete the message queue: %d\n", id);

        if (msgctl(id, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(EXIT_FAILURE);
        }
    }
}
