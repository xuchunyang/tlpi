/* 118-svshm-demo.c --- System V 共享内存 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/stat.h>           /* S_IRUSR */
int
getInt(char *s)
{
    errno = 0;
    int result = strtol(s, NULL, 10);
    if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }
    return result;
}

int
main(int argc, char* argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("usage: %s [shmid] [delete]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 1) {            /* 创建 shared memory 并写入内容 */
        char buf[1024] = "HELLO, WORLD!";
        int id = shmget(IPC_PRIVATE, sizeof buf,
                        S_IRUSR | S_IWUSR
                        // | SHM_R | SHM_W
            );
        if (id == -1) {
            perror("shmget");
            exit(EXIT_FAILURE);
        }

        printf("Created Shared Memory ID=%d\n", id);

        printf("Writing '%s' to the shared memory\n", buf);

        char *shmp = shmat(id, NULL, 0);
        if (shmp == (void*) -1) {
            perror("shmat");
            exit(EXIT_FAILURE);
        }
        memcpy(shmp, buf, sizeof buf);
    } else if (argc == 2)   {   /* 读取 shared memory 的内容 */
        int id = getInt(argv[1]);
        char *shmp = shmat(id, NULL, 0);
        if (shmp == (void*) -1) {
            perror("shmat");
            exit(EXIT_FAILURE);
        }
        printf("共享内存 id=%d 的数据为 '%s'\n", id, shmp);
    } else {                    /* 删除 shared memory */
        int id = getInt(argv[1]);
        printf("Delete shared memory %d\n", id);
        if (shmctl(id, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(EXIT_FAILURE);
        }
    }
}
