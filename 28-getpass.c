/* 28-getpass.c --- getpass */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
int
main()
{
    char* passwd = getpass("Enter your password: ");
    printf("Your password is: %s\n", passwd);
    memset(passwd, 0, strlen(passwd));
    assert(*passwd == 0);
}
