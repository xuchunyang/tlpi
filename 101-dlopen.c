/* 101-dlopen.c --- dlopen() & dlsym() */

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int foo()
{
    return 42;
}

int
main()
{
    void* lib_handler = dlopen("mod/libfoo.so", RTLD_LAZY);
    if (lib_handler == NULL) {
        printf("dlopen: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    int *glob = dlsym(lib_handler, "glob");
    if (glob == NULL) {
        printf("dlsym: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    printf("glob = %d\n", *glob);

    int (*forty_two)();
    *((void**) &forty_two) = dlsym(lib_handler, "forty_two");
    if (forty_two == NULL) {
        printf("dlsym: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    printf("forty_two() = %d\n", forty_two());

    dlclose(lib_handler);
}
