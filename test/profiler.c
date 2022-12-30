#define SHEEP_PROFILER_IMPLEMENTATION
#include "../profiler.h"
#define _GNU_SOURCE
#include <string.h>

int fib(int n)
{
    if (n <= 2) return 1;
    return fib(n - 1) + fib(n - 2);
}

static char buf[1<<6];

int main()
{
    Profiler pf = {0};
    Profiler p = profiler_new();
    for (int i = 0; i < 40; i += 5)
    {
        snprintf(buf, sizeof buf, "fib(%d)", i);
        profiler_start(p, buf);
        fib(i);
        profiler_stop(p);
    }

    FILE *fp = fopen("/dev/null", "w");
    profiler_start(p, "IO");
    for (int i = 0; i < 100000; i++)
        fprintf(fp, "Kowaaaaaa");
    profiler_stop(p);
    fclose(fp);

    profiler_print(p);
    profiler_free(p);
    return 0;
}

