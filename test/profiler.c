#define SHEEP_PROFILER_IMPLEMENTATION
#include "../profiler.h"
#define _GNU_SOURCE
#include <string.h>

int fib(int n)
{
    if (n <= 2) return 1;
    return fib(n - 1) + fib(n - 2);
}

static char buf[2048];

int main()
{
    struct profiler pf = { 0 };
    for (int i = 0; i < 30; i += 5)
    {
        snprintf(buf, sizeof buf, "fib(%d)", i);
        profiler_start(&pf, buf);
        fib(i);
        profiler_stop(&pf, buf);
    }

    FILE *fp = fopen("/dev/null", "w");
    profiler_start(&pf, "IO");
    for (int i = 0; i < 100000; i++)
        fprintf(fp, "Kowaaaaaa");
    profiler_stop(&pf, "IO");
    fclose(fp);

    profiler_print(&pf);
    profiler_cleanup(&pf);
    return 0;
}

