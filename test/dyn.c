#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../dynarray.h"
#include <assert.h>

int main() {
    int *a = arrnew;
    arrpush(a, 5);
    arrpush(a, 5);
    assert(arrlen(a) == 2);
    assert(arrcap(a) == 4);
    arrpush(a, 5);
    arrpush(a, 5);
    arrpush(a, 5);
    assert(arrcap(a) == 8);
    arrfree(a);
    a = arrnew;
    for (int i = 0; i < 10; i++) {
        arrpush(a, i);
        assert(a[i] == i);
    }
}
