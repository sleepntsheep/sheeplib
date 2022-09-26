/* dynarray.h - v0.02 - sleepntsheep 2022
 * dynarray.h is single-header library for
 * nice dynamic array in C, similar to 
 * std::vector in C++ 
 *
 * inspired by https://github.com/nothings/stb/blob/master/stb_ds.h 
 *
 * Instruction & How to use
 *
 *      #define SHEEP_DYNARRAY_IMPLEMENTATION 
 *      #include "dynarray.h"
 *      T* arr = dynarray_new(T);
 *      dynarray_push(arr, X);
 *
 * If you don't want arrfnc shorthand
 * to dynarray_fnc then define SHEEP_DYNARRAY_NOSHORTHAND
 * to avoid name collision
 *
 * If you want to use custom alloactor,
 * define DYNARRAY_MALLOC and DYNARRAY_REALLOC to them
 * before including dynarray.h
 *
 * How it works
 *
 * Similar to stb_ds.h, this allocates extra memory for
 * type dynarray_info_t in front of the array which 
 * act as a header that store information about the array,
 * specifically length, capacity, membsize.
 *
 *
 * As opposed too using NULL to create new dynarray
 * and checking for NULL reference on every function/macro
 * like in stb_ds.h
 * I decided to have dynarray_new macro for creating dynarray
 * While the redundance type may look a bit ugly
 * It make the code clearer that arr is dynamic array, 
 * not just normal pointer
 * And it make dynarray.h more maintainable
 *
 * Notes :
 *  Due to limitation of the C language, 
 *  almost all macros here are not type-safe
 *  this is a hacky solution for dynamic array,
 *  but it provide nicer interface than other approach
 *  for example arr[i] rather than vec_at(arr, i)
 *  moreover it can be used for any type
 * 
 * Documentations 
 *
 *  arrnew(T) : 
 *      create and return new dynamic array with type T
 *
 *  arrfree(T) :
 *      free dynamic array
 *
 *  arrpush(A, x) :
 *      grow array if needed and
 *      push x into the end of array A
 * 
 *  arrpop(A) :
 *      remove last element of array A and return it
 *
 *  arrtop(A) :
 *      return last element of array, without removing it
 *
 *  arrlen(A) :
 *      return length of array A
 *
 *  arrcap(A) :
 *      return capacity of array A
 *
 *  arrins(A, idx, x) :
 *      grow array if needed and
 *      insert x into index idx of array A
 *      moving all following elements one index to the right
 *
 *  arrdel(A, idx) :
 *      remove element at index idx of array A
 *      moving all following elements one index to the left
 *
 *  arrsetlen(A, len) :
 *      set length of array A to *at least* len
 *      allocating more memory if needed
 *      *memory will be uninitialized
 *
 *  arrsetcap(A, cap) :
 *      set capacity of array A to *at least* cap
 *      allocating more memory if needed
 *      *memory will be uninitialized
 *
 * arrsort *won't* be implemented, just use qsort from stdlib
 * this library *cannot* be used in C++,
 * due to C++ not allowing implicit pointer conversion
 * you shouldn't use this in C++ anyway, use std::vector
 * */

#pragma once

#ifndef SHEEP_DYNARRAY_H
#define SHEEP_DYNARRAY_H

#ifndef DYNARRAY_REALLOC
#include <stdlib.h>
#define DYNARRAY_REALLOC realloc
#endif  /* DYNARRAY_REALLOC */

#ifndef DYNARRAY_MALLOC
#include <stdlib.h>
#define DYNARRAY_MALLOC malloc
#endif  /* DYNARRAY_MALLOC */

#ifndef DYNARRAY_FREE
#include <stdlib.h>
#define DYNARRAY_FREE free
#endif  /* DYNARRAY_FREE */


typedef struct dynarray_info_s {
    long length;
    long capacity;
} dynarray_info_t;

#define DYNARRAY_MIN_CAPACITY 4

#ifndef SHEEP_DYNARRAY_NOSHORTHAND

#define arrnew dynarray_new
#define arrfree dynarray_free
#define arrlen dynarray_len
#define arrcap dynarray_cap
#define arrdel dynarray_del
#define arrpop dynarray_pop
#define arrtop dynarray_top
#define arrpush dynarray_push
#define arrsetlen dynarray_setlen
#define arrsetcap dynarray_setcap
#define arrins dynarray_ins

#endif /* SHEEP_DYNARRAY_NOSHORTHAND */

void *dynarray_growf(void *a, long cap, long membsize);
static long dynarray_first_2n_bigger_than(long x);

#define dynarray_new ((void*)0)
#define dynarray_ensure_empty(A, n) (dynarray_setcap((A), dynarray_first_2n_bigger_than(dynarray_len(A) + (n))))
#define dynarray_info(A) (((dynarray_info_t*)(A))-1)
#define dynarray_top(A) ((A)[dynarray_info(A)->length-1])
#define dynarray_pop(A) ((A)[--dynarray_info(A)->length])
#define dynarray_push(A,x) \
    do { \
        (A) = dynarray_ensure_empty((A), 1); \
        (A)[dynarray_len(A)] = (x); \
        dynarray_info(A)->length++; \
    } while(0)
#define dynarray_ins(A,idx,x) \
    do { \
        (A) = dynarray_ensure_empty((A), 1); \
        for (long i = idx; i <= dynarray_len(A); i++) \
            (A)[i+1] = (A)[i]; \
        (A)[idx] = (x); \
        dynarray_info(A)->length++; \
    } while(0)
#define dynarray_del(A,idx) \
    do { \
        for (long i = idx; i < dynarray_len(A) - 1; i++) \
            A[i] = A[i+1]; \
        dynarray_info(A)->length--; \
    } while(0)
#define dynarray_free(A) ((A) ? DYNARRAY_FREE(dynarray_info(A)) : 0)
#define dynarray_len(A) ((A) ? dynarray_info(A)->length : 0)
#define dynarray_cap(A) ((A) ? dynarray_info(A)->capacity : DYNARRAY_MIN_CAPACITY)
#define dynarray_setcap(A, n) dynarray_growf((A), (n), sizeof(*(A)))
#define dynarray_setlen(A, n) ((A) = dynarray_setcap((A), (n)), dynarray_info(A)->length = (n), (A))

#endif /* SHEEP_DYNARRAY_H */

#ifdef SHEEP_DYNARRAY_IMPLEMENTATION 
void *dynarray_growf(void *a, long cap, long membsize) {
    /* simply ensure array have capacity of 
     * at least cap */
    if (!a) {
        a = ((dynarray_info_t*)
                DYNARRAY_MALLOC(sizeof(dynarray_info_t)
                    + membsize * DYNARRAY_MIN_CAPACITY))+1;
        dynarray_info(a)->capacity = DYNARRAY_MIN_CAPACITY;
        dynarray_info(a)->length = 0;
    }
    if (cap <= dynarray_cap(a))
        return a;
    void *b;
    b = ((dynarray_info_t*)
            DYNARRAY_REALLOC(dynarray_info(a), sizeof(dynarray_info_t) + cap * membsize))+1;
    dynarray_info(b)->capacity = cap;
    return b;
}

static long dynarray_first_2n_bigger_than(long x) {
    long ret = 1;
    while (ret < x)
        ret <<= 1;
    return ret;
}

#endif /* SHEEP_DYNARRAY_IMPLEMENTATION */
