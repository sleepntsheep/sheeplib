#ifndef SHEEP_SMATH_H
#include <stdint.h>

#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "dynarray.h"

#define MAX_1_SLOT 1000000000

typedef uint32_t * bigint;

bigint bigint_new();
bigint bigint_add(bigint a, bigint b);
bigint bigint_sub(bigint a, bigint b);
bigint bigint_mul(bigint a, bigint b);
bigint bigint_div(bigint a, bigint b);

#define MINMAXTYPES \
    X(int, i) \
    X(unsigned int, u) \
    X(long int, l) \
    X(long unsigned int, lu) \
    X(float, f) \
    X(double, lf)

#define X(a,b) \
        a sheep_min##b(a n1, a n2); a sheep_max##b(a n1, a n2);
    MINMAXTYPES
#undef X

#endif /* SHEEP_SMATH_H */

#ifdef SHEEP_SMATH_IMPLEMENTATION
#include <string.h>

#define X(a,b) \
        a sheep_min##b(a n1, a n2) { return n1 < n2 ? n1 : n2; } \
        a sheep_max##b(a n1, a n2) { return n1 > n2 ? n1 : n2; }
    MINMAXTYPES
#undef X


bigint bigint_new() {
    return dynarray_new(uint32_t);
}

bigint bigint_from(const char *s) {
    bigint ret = bigint_new();
    while (s) {
        uint32_t cur = 0;
        while (s && cur < MAX_1_SLOT) {
            cur *= 10;
            cur += *s++;
        }
        arrpush(ret, cur);
    }
    return ret;
}

bigint bigint_add(bigint a, bigint b) {
    size_t dn = sheep_minlu(arrlen(a), arrlen(b));
    size_t mdn = sheep_maxlu(arrlen(a), arrlen(b));
    bigint ret = bigint_new();
    uint32_t carry = 0;
    for (int i = 0; i < dn; i++) {
        uint32_t added = a[i] + b[i] + carry;
        arrpush(ret, added % MAX_1_SLOT);
        carry = added / MAX_1_SLOT;
    }
}

char* bigint_to_str(bigint a) {
    size_t length = arrlen(a) * 

}

#endif
