#include <stdio.h>
#define SPOOL_H_IMPLEMENTATION
#include "../spool.h"

static char POOL[1<<20] = { 0 };

int main() {
    spool pool;
    spool_init(&pool, POOL, sizeof POOL);
    int *a = spool_alloc(&pool, sizeof *a);
    *a = 4;
    printf("%p : %d\n", a, *a);
    spool_free(&pool, a);
    return 0;
}

