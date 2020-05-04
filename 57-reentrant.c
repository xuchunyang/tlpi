/* 57-reentrant.c --- 可重入 */

/* crypt(3) 使用静态分配变量，不可重入 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#define _XOPEN_SOURCE           /* crypt */
#include <unistd.h>

static int handled = 0;

static void
handler(int sig __attribute__((unused)))
{
    handled++;
    crypt("def", "xx");
}

int
main()
{
    signal(SIGINT, handler);
    char* cr1 = strdup(crypt("abc", "xx"));
    int callNum = 0;
    int mismatched = 0;
    for(;;callNum++) {
        if (strcmp(crypt("abc", "xx"), cr1) != 0) {
            mismatched++;
            printf("total = %d, handled = %d, mismatched = %d\n",
                   callNum, handled, mismatched);
        }
    }
}
