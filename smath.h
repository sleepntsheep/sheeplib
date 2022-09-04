#ifndef SHEEP_SMATH_H
#include <stdint.h>

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

#define X(a,b) \
        a sheep_min##b(a n1, a n2) { return n1 < n2 ? n1 : n2; } \
        a sheep_max##b(a n1, a n2) { return n1 > n2 ? n1 : n2; }
    MINMAXTYPES
#undef X

#endif
