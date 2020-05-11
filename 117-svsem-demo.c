/* 117-svsem-demo.c --- System V 信号量 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h> // S_IRUSR
#include <errno.h>
#include <time.h>
#include <unistd.h>

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

char*
now(char* fmt)
{
    time_t t = time(NULL);
    if (t == (time_t) -1) {
        perror("time");
        exit(EXIT_FAILURE);
    }

    struct tm* tt = localtime(&t);
    if (tt == NULL) {
        perror("localtime");
        exit(EXIT_FAILURE);
    }

    static char buf[1024];
    if (strftime(buf, sizeof buf, fmt, tt) == 0) {
        perror("strftime");
        exit(EXIT_FAILURE);
    }

    return buf;
}

/* 为 Linux 自己定义 union semun，因为 glibc 可能不定义，见 p796  sys/sem.h */
union my_semun {
    int             val;            /* value for SETVAL */
    struct semid_ds *buf;           /* buffer for IPC_STAT & IPC_SET */
    unsigned short  *array;         /* array for GETALL & SETALL */
};


int
main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s init-value\n"
               "       %s semid operation\n",
               argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {            /* 创建和初始化信号量 */
        int id = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
        if (id == -1) {
            perror("semget");
            exit(EXIT_FAILURE);
        }

        union my_semun arg = {
            .val = getInt(argv[1]),
        };
        if (semctl(id, 0, SETVAL, arg) == -1) {
            perror("semctl");
            exit(EXIT_FAILURE);
        }

        printf("Semphore ID = %d\n", id);
    } else {
        int id = getInt(argv[1]);
        if (strcmp(argv[2], "delete") == 0) { /* 删除信号量 */
            printf("Deleting Semphore ID = %d\n", id);
            if (semctl(id, 0, IPC_RMID) == -1) {
                perror("semctl");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[2], "info") == 0) { /* 打印信号量中所有信号量的当前值 */

            /* 获得信号量的总数 semid_ds -> sem_nsems */
            struct semid_ds ids;
            union my_semun arg1 = {
                .buf = &ids,
            };
            if (semctl(id, 0, IPC_STAT, arg1) == -1) {
                perror("semctl");
                exit(EXIT_FAILURE);
            }

            printf("Semphore %d 中总个有 %d 个信号量，它们的值分别为:\n",
                   id,
                   ids.sem_nsems);

            unsigned short* vals = malloc(ids.sem_nsems * sizeof(unsigned short));
            if (vals == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            union my_semun arg2 = {
                .array = vals,
            };
            if (semctl(id, 0, GETALL, arg2) == -1) {
                perror("semctl");
                exit(EXIT_FAILURE);
            }
            printf("Sem\tVale\n");
            for (int i = 0; i < ids.sem_nsems; i++)
                printf("%d\t%d\n", i, vals[i]);
        } else {
            struct sembuf sop = {
                .sem_num = 0,
                .sem_op  = getInt(argv[2]),
                .sem_flg = 0,
            };
            printf("%ld: about to semop at %s\n", (long) getpid(), now("%F %T"));
            if (semop(id, &sop, 1) == -1) {
                perror("semop");
                exit(EXIT_FAILURE);
            }
            printf("%ld: semop completed at %s\n", (long) getpid(), now("%F %T"));
        }
    }
}
