/**
 * @file dynarray.h
 * @brief Generic dynamic array
 *
 * dynarray.h - v0.04 - sleepntsheep 2022
 * dynarray.h is single-header library for
 * dynamic array in C, similar to std::vector in C++
 *
 * inspired by https://github.com/nothings/stb/blob/master/stb_ds.h
 *
 * Instruction & How to use
 *
 *      #define SHEEP_DYNARRAY_IMPLEMENTATION
 *      #include "dynarray.h"
 *      T* arr = dynarray_new;
 *      dynarray_push(arr, X);
 *      T elem = arr[0];
 *      dynarray_free(arr);
 *
 * There are arrmethod functions counterpart for each
 * dynarray_method functions.
 * If you don't want those, define SHEEP_DYNARRAY_NOSHORTHAND
 *
 * If you want to use custom alloactor,
 * define DYNARRAY_MALLOC and DYNARRAY_REALLOC to them
 * before including dynarray.h
 *
 * How it works
 *
 * Similar to stb_ds.h, this allocates extra memory for
 * type struct _dynarray_info in front of the array which
 * act as a header that store information about the array,
 * specifically length, capacity.
 * This mean you can't free() the dynamic array normally,
 * Since you need to calculate the header's pointer location
 * so you can use dynarray_free() for that
 *
 * dynarray_new is NULL or (void*)0.
 * It exists for readability of the code, to make it clear that
 * the pointer is a dynamic array.
 *
 *
 * - arrsort *won't* be implemented, just use qsort from stdlib
 *
 * - this library *cannot* be used in C++,
 * due to C++ not allowing implicit pointer conversion
 * you shouldn't use this in C++ anyway, use std::vector
 *
 *
 * Change Notes:
 * v 0.0.4:
 *  - dynarray_setcap() now modify the pointer by itself, no need to do
 *      A = dynarray_setcap(A, newcap)
 *      just do
 *      dynarray_setcap(A, newcap)
 *  - C++ support, via some preprocessing things and template
 * */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SHEEP_DYNARRAY_H
#define SHEEP_DYNARRAY_H

#ifndef DYNARRAY_REALLOC
#include <stdlib.h>
#define DYNARRAY_REALLOC realloc
#endif /* DYNARRAY_REALLOC */

#ifndef DYNARRAY_MALLOC
#include <stdlib.h>
#define DYNARRAY_MALLOC malloc
#endif /* DYNARRAY_MALLOC */

#ifndef DYNARRAY_FREE
#include <stdlib.h>
#define DYNARRAY_FREE free
#endif /* DYNARRAY_FREE */

#ifndef DYNARRAY_MEMCPY
#include <string.h>
#define DYNARRAY_MEMCPY memcpy
#endif /* DYNARRAY_MEMCPY */

#ifndef DYNARRAY_ASSERT
#include <assert.h>
#define DYNARRAY_ASSERT assert
#endif /* DYNARRAY_ASSERT */

struct _dynarray_info {
    size_t length;   /* dynamic array length */
    size_t capacity; /* dynamic array capacity */
};

#define DYNARRAY_MIN_CAPACITY 4

#ifndef SHEEP_DYNARRAY_NOSHORTHAND

#define arrnew dynarray_new
#define arrfree dynarray_free
#define arrlen dynarray_len
#define arrcap dynarray_cap
#define arrdel dynarray_del
#define arrpop dynarray_pop
#define arrback dynarray_back
#define arrpush dynarray_push
#define arrsetlen dynarray_setlen
#define arrsetcap dynarray_setcap
#define arrins dynarray_ins
#define arrpushn dynarray_pushn

#endif /* SHEEP_DYNARRAY_NOSHORTHAND */

void *dynarray_growf(void *a, size_t cap, size_t membsize);
size_t dynarray_first_2n_bigger_than(size_t x);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/* C++ is bad and don't have implicit pointer conversion so we're stuck with
 * this */
template <class T>
T *dynarray_growf_wrapper(T *a, size_t cap, size_t membsize) {
    return (T *)dynarray_growf(a, cap, membsize);
}
#else
#define dynarray_growf_wrapper dynarray_growf
#endif /* __cplusplus */

/**
 * @brief NULL, this is to make code more clear that the pointer is dynamic
 * array
 * @return pointer to the dynamic array
 */
#define dynarray_new NULL
/**
 * @brief if space is not enough for adding n more new elements, grow array to
 * smallest power of two that fit
 * @param A dynamic array
 * @param n amount of elements to make space for
 * @return pointer to the dynamic array (might be moved by realloc)
 */
#define dynarray_ensure_empty(A, n)                                            \
    (dynarray_setcap((A), dynarray_first_2n_bigger_than(dynarray_len(A) + (n))))
/**
 * @brief private: get pointer to information about dynamic array
 * @param A dynamic array
 * @return pointer to header of that dynamic array
 */
#define dynarray_info(A) (((struct _dynarray_info *)(A)) - 1)
/**
 * @brief get last element of dynamic array
 * @param A dynamic array
 * @return last element of dynamic array
 */
#define dynarray_back(A) ((A)[dynarray_info(A)->length - 1])
/**
 * @brief pop last element of dynamic array
 * @param A dynamic array
 * @return last element of dynamic array
 */
#define dynarray_pop(A) ((A)[--dynarray_info(A)->length])
/**
 * @brief push element to end of array
 * @param A dynamic array
 * @param x new element to add to the array
 */
#define dynarray_push(A, x)                                                    \
    do {                                                                       \
        dynarray_ensure_empty((A), 1);                                         \
        (A)[dynarray_len(A)] = (x);                                            \
        dynarray_info(A)->length++;                                            \
    } while (0)
/**
 * @brief insert element to index idx of array, moving other elements to the
 * right if needed
 * @param A dynamic array
 * @param idx position to insert the element at
 * @param x new element
 */
#define dynarray_ins(A, idx, x)                                                \
    do {                                                                       \
        dynarray_ensure_empty((A), 1);                                         \
        for (size_t i = idx; i <= dynarray_len(A); i++)                        \
            (A)[i + 1] = (A)[i];                                               \
        (A)[idx] = (x);                                                        \
        dynarray_info(A)->length++;                                            \
    } while (0)
/**
 * @brief delete element at index idx from array, moving all latter element to
 * the left
 * @param A dynamic array
 * @param idx index of element to delete
 */
#define dynarray_del(A, idx)                                                   \
    do {                                                                       \
        for (size_t i = idx; i < dynarray_len(A) - 1; i++)                     \
            A[i] = A[i + 1];                                                   \
        dynarray_info(A)->length--;                                            \
    } while (0)
/**
 * @brief free dynamic array
 */
#define dynarray_free(A) ((A) ? DYNARRAY_FREE(dynarray_info(A)), 0 : 0)
/**
 * @brief return length of dynamic array
 * @param A dynamic array
 * @return length of dynamic array (size_t)
 */
#define dynarray_len(A) ((A) ? dynarray_info(A)->length : 0)
/**
 * @brief return capacity of dynamic array
 * @param A dynamic array
 * @return capacity of dynamic array (size_t)
 */
#define dynarray_cap(A)                                                        \
    ((A) ? dynarray_info(A)->capacity : DYNARRAY_MIN_CAPACITY)
/**
 * @brief set capacity of dynamic array to n, realloc if needed
 * @param A dynamic array
 * @param n new capacity
 * @return pointer to dynamic array (might be moved by realloc)
 */
#define dynarray_setcap(A, n)                                                  \
    (A = dynarray_growf_wrapper((A), (n), sizeof(*(A))))
/**
 * @brief set length of dynamic array to n, set capacity if needed
 * @param A dynamic array
 * @param n new length
 * @return pointer to dynamic array (might be moved by realloc)
 */
#define dynarray_setlen(A, n)                                                  \
    (dynarray_setcap((A), (n)), dynarray_info(A)->length = (n))
/**
 * @brief push n element starting at pointer p to dynamic array
 * @param A dynamic array
 * @param p location of elements
 * @param n count of elements to push
 *
 * sizeof type of element that p is pointing to ***must*** be
 * equal to sizeof element of dynamic array, and n is count of
 * elements, not total size, not bytes to copy
 */
#define dynarray_pushn(A, p, n)                                                \
    DYNARRAY_ASSERT(sizeof(*(p)) == sizeof(*(A)));                             \
    dynarray_ensure_empty((A), (n));                                           \
    DYNARRAY_MEMCPY((A) + dynarray_len(A), p, sizeof *(p)*n);                  \
    dynarray_info(A)->length += n;

#endif /* SHEEP_DYNARRAY_H */

#ifdef SHEEP_DYNARRAY_IMPLEMENTATION

void *dynarray_growf(void *a, size_t cap, size_t membsize) {
    if (a == NULL) {
        a = ((struct _dynarray_info *)DYNARRAY_MALLOC(
                sizeof(struct _dynarray_info) +
                membsize * DYNARRAY_MIN_CAPACITY)) +
            1;
        dynarray_info(a)->capacity = DYNARRAY_MIN_CAPACITY;
        dynarray_info(a)->length = 0;
    }
    if (cap <= dynarray_cap(a))
        return a;
    void *b;
    b = ((struct _dynarray_info *)DYNARRAY_REALLOC(
            dynarray_info(a), sizeof(struct _dynarray_info) + cap * membsize)) +
        1;
    dynarray_info(b)->capacity = cap;
    return b;
}

size_t dynarray_first_2n_bigger_than(size_t x) {
    size_t ret = 1;
    while (ret < x)
        ret <<= 1;
    return ret;
}

#endif /* SHEEP_DYNARRAY_IMPLEMENTATION */
