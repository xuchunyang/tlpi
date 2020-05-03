/* 42-fts.c --- file tree stream */

#include <errno.h>
#include <string.h>
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char* argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s dirname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FTS* ftsp = fts_open(&argv[1],
                         FTS_PHYSICAL,
                         NULL);
    if (ftsp == NULL) {
        perror("fts_open");
        exit(EXIT_FAILURE);
    }

    FTSENT* ent;
    errno = 0;
    while ((ent = fts_read(ftsp))) {
        switch (ent->fts_info) {
        case FTS_DP:
            /* A directory being visited in postorder */
            break;
        default:
            puts(ent->fts_path);
            break;
        }
        /* printf("%s %p\n", */
        /*        ent->fts_path, */
        /*        // ent->fts_name, */
        /*        (void*) ent->fts_link */
        /*     ); */
        // FTSENT* ent2 = fts_children(ftsp, FTS_NAMEONLY);
        // ent2->fts_link
    }
    if (ent == NULL && errno != 0) {
        perror("fts_read");
        exit(EXIT_FAILURE);
    }

    if (fts_close(ftsp) == -1) {
        perror("fts_close");
        exit(EXIT_FAILURE);
    }
}
