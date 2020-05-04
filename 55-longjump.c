/* 55-longjump.c --- nonlocal goto */
#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;

static void
f2()
{
    longjmp(env, 2);
}

static void
f1(int argc)
{
    if (argc > 1)
        f2();
    else
        longjmp(env, 1);
}

int
main(int argc,
     char* argv[] __attribute__((unused)))
{
    switch (setjmp(env)) {
    case 0:
        printf("Calling f1() after setjmp\n");
        f1(argc);
        break;

    case 1:
        printf("jump back from f1()");
        break;

    case 2:
        printf("jump back from f2()");
        break;
    }
}
