/* 47-inotify.c --- Linux inotify */

#ifdef __linux__

#include <limits.h> // NAME_MAX
#include <sys/inotify.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void             /* Display information from inotify_event structure */
displayInotifyEvent(struct inotify_event *i)
{
    printf("    wd =%2d; ", i->wd);
    if (i->cookie > 0)
        printf("cookie =%4d; ", i->cookie);

    printf("mask = ");
    if (i->mask & IN_ACCESS)        printf("IN_ACCESS ");
    if (i->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
    if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
    if (i->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
    if (i->mask & IN_CREATE)        printf("IN_CREATE ");
    if (i->mask & IN_DELETE)        printf("IN_DELETE ");
    if (i->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
    if (i->mask & IN_IGNORED)       printf("IN_IGNORED ");
    if (i->mask & IN_ISDIR)         printf("IN_ISDIR ");
    if (i->mask & IN_MODIFY)        printf("IN_MODIFY ");
    if (i->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
    if (i->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
    if (i->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
    if (i->mask & IN_OPEN)          printf("IN_OPEN ");
    if (i->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
    if (i->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
    printf("\n");

    if (i->len > 0)
        printf("        name = %s\n", i->name);
}

int
main(int argc, char* argv[])
{
    int inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        int wd = inotify_add_watch(inotifyFd, argv[i], IN_ALL_EVENTS);
        if (wd == -1) {
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }
        printf("Watching %s using wd %d\n",
        argv[i], wd);
    }

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

    char buf[BUF_LEN];
    for(ssize_t numRead;;) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (numRead == 0) {
            printf("read() from inotify fd returned 0!");
            exit(EXIT_FAILURE);
        }

        printf("Read %ld bytes from inotify fd\n", numRead);

        for (char *p = buf; p < buf + numRead;) {
            struct inotify_event* event = (struct inotify_event*) p;
            
            displayInotifyEvent(event);
            // printf("'%s'\n", event->name);
            
            p += (sizeof event) + event->len;
        }
    }
}

#else
/* Ignored */
int main(){}
#endif
