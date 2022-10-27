#define FABE64_IMPLEMENTATION
#include "../fabe64.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char o[8096];
char o2[8096];
int main()
{
    fabe64_encode("foobar", 6, o, 8096);
    puts(o);
    o[fabe64_encode("fooba", 5, o, 8096)] = 0;
    puts(o);
    fabe64_encode("foob", 4, o, 8096);
    puts(o);
    char *a = "ShinLena is the best ship ever ever!!! Don't ever not love shinlena 86 86 86 ";
    o[fabe64_encode(a, strlen(a), o, 8096)] = 0;
    puts(o);

    char *o2 = calloc(8096, 1);
    o2[fabe64_decode(o, o2, 8096)] = 0;
    puts(o2);

    return 0;
}

