/* 34-stat.c --- stat */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

static void
printStat(const char* pathname)
{
    struct stat statbuf;
    if (stat(pathname, &statbuf) == -1) {
        perror("stat");
        return;
    }
    printf("%s:\n", pathname);
    printf("DEVICE ID:\t%d\n", statbuf.st_dev);
    printf("inode number:\t%llu\n", statbuf.st_ino);
    printf("hard link number:\t%d\n", statbuf.st_nlink);
    printf("uid:\t%d\n", statbuf.st_uid);
    printf("gid:\t%d\n", statbuf.st_gid);
    printf("DEVICE ID st_rdev:\t%d\n", statbuf.st_rdev); /* 设备文件 */
    printf("size (bytes):\t%lld\n", statbuf.st_size);

    printf("file type:\t");
    switch (statbuf.st_mode & S_IFMT) {
    case S_IFREG: printf("regular file\n"); break;
    case S_IFDIR: printf("directory\n"); break;
    case S_IFCHR: printf("character device\n"); break;
    case S_IFBLK: printf("block device\n"); break;
    case S_IFIFO: printf("FIFO or pipe\n"); break;
    default: printf("unknown file type\n"); break;
    }

    printf("mode:\t");
    printf("%c%c%c\n",
           statbuf.st_mode & S_IRUSR ? 'r' : '-',
           statbuf.st_mode & S_IWUSR ? 'w' : '-',
           statbuf.st_mode & S_IXUSR ? 'x' : '-');


    printf("time of last access: %s",
           ctime(&statbuf.st_atimespec.tv_sec));
    printf("time of last data modification: %s",
           ctime(&statbuf.st_ctimespec.tv_sec));
    printf("time of last file status change: %s",
           ctime(&statbuf.st_mtimespec.tv_sec));

    printf("\n");
}

int
main(int argc, char* argv[])
{
    printf("文件类型位置 S_IFMT: 0x%X\n", S_IFMT);
    printf("u+r S_IRUSR: 0x%x\n", S_IRUSR);
    printf("u+w S_IWUSR: 0x%x\n", S_IWUSR);
    printf("u+x S_IXUSR: 0x%x\n", S_IXUSR);

    for (int i = 0; i < argc; i++)
        printStat(argv[i]);
}
