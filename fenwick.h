#ifndef SHEEP_FENWICK_H
#define SHEEP_FENWICK_H
#include <stdlib.h>

#define FENWICK_DECL(type, zero, updatefnc) \
    typedef struct { \
        type *a; \
        long size; \
    } fenwick_##type##_t; \
\
    fenwick_##type##_t fenwick_##type##_new(type *a, long size); \
    void fenwick_##type##_update(fenwick_##type##_t t, long i, type x); \
    type fenwick_##type##_query(fenwick_##type##_t t, long i); \
\
    fenwick_##type##_t fenwick_##type##_new(type *a, long size) { \
        fenwick_##type##_t t; \
        t.a = malloc(sizeof(type) * (size+1)); \
        t.size = size; \
        for (long i = 1; i <= size; i++) \
            t.a[i] = zero; \
        for (long i = 1; i <= size; i++) \
            fenwick_##type##_update(t, i, a[i]); \
        return t; \
    } \
\
    void fenwick_##type##_update(fenwick_##type##_t t, long i, type x) { \
        while (i <= t.size) { \
            t.a[i] = updatefnc(t.a[i], x); \
            i += i & -i; \
        } \
    } \
\
    type fenwick_##type##_query(fenwick_##type##_t t, long i) { \
        type sum = zero; \
        while (i > 0) { \
            sum = updatefnc(t.a[i], sum); \
            i -= i & -i; \
        } \
        return sum; \
    }


#endif /* SHEEP_FENWICK_H */
