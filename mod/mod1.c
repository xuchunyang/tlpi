/* mod1.c  --- a module for testing static and shared library */


int glob = 42;

int
forty_two()
{
    return 42;
}


/* Local Variables: */
/* compile-command: "cc -fPIC mod1.c -shared -o libfoo.so" */
/* End: */
