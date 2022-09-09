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
 * If you want to use long 
 * dynarray_fnc instead of arrfnc
 * you can define SHEEP_DYNARRAY_NOSHORTHAND
 * to avoid name collision
 *
 * If you want to use custom alloactor,
 * define malloc and realloc to them
 * before including dynarray.h
 *
 * How it works
 *
 * Similar to stb_ds.h, this allocate extra memory for
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
 *  this is a hacky solution for dynamic array
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
 * due to C++ not allowing C++ not allowing implicit pointer conversion
 * you shouldn't use this in C++ anyway, use std::vector
 * */

#pragma once

#ifndef SHEEP_DYNARRAY_H
#define SHEEP_DYNARRAY_H

#include <string.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct dynarray_info_s {
    size_t length;
    size_t capacity;
    size_t membsize;
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

#define dynarray_info(A) (((dynarray_info_t*)(A))-1)
#define dynarray_new(T) ((T*)(_dynarray_new(sizeof(T))))
#define dynarray_pop _dynarray_pop
#define dynarray_top _dynarray_top
#define dynarray_push _dynarray_push
#define dynarray_ins _dynarray_ins

void   dynarray_del(void* a, size_t idx);
void   dynarray_free(void* a);
size_t dynarray_len(void* a);
size_t dynarray_cap(void* a);
size_t dynarray_membsize(void *a);
void   dynarray_setlen(void* a, size_t len);
void  *dynarray_setcap(void* a, size_t cpa);
void  *_dynarray_new(size_t membsize);
void  *dynarray_ensure_empty(void* a, size_t n);


#endif /* SHEEP_DYNARRAY_H */

#ifdef SHEEP_DYNARRAY_IMPLEMENTATION 

#define _dynarray_top(A) \
    (A)[dynarray_info(A)->length-1]

#define _dynarray_pop(A) \
    (A)[--dynarray_info(A)->length]

#define _dynarray_push(A,x) \
    do { \
        A = dynarray_ensure_empty((A), 1); \
        (A)[arrlen(A)] = (x); \
        dynarray_info(A)->length++; \
    } while(0)

#define _dynarray_ins(A,idx,x) \
    do { \
        A = dynarray_ensure_empty((A), 1); \
        /* pointer arithmetic already multiply idx with membsize for us in dest and src */ \
        memmove((A) + (idx+1), \
                (A) + (idx), \
                (dynarray_len(A) - idx) * dynarray_membsize(A)); \
        A[idx] = (x); \
        dynarray_info(A)->length++; \
    } while(0)

void *dynarray_ensure_empty(void* a, size_t n) {
    /* make sure space is empty enough for at least n more member */
    size_t cap = dynarray_cap(a);
    while (cap - dynarray_len(a) < n)
        cap *= 2;
    return dynarray_setcap(a, cap);
}

void dynarray_del(void* a, size_t idx) {
    dynarray_info_t *info = dynarray_info(a);
    memmove((char*)a + (idx)   * info->membsize,
            (char*)a + (idx+1) * info->membsize,
            (info->length - idx - 1) * info->membsize);
    info->length--;
}

void dynarray_free(void* a) {
    /* free dynamic array */
    free(dynarray_info(a));
}

size_t dynarray_len(void* a) {
    /* return length */
    return dynarray_info(a)->length;
}

size_t dynarray_membsize(void *a) {
    return dynarray_info(a)->membsize;
}

size_t dynarray_cap(void* a) {
    /* return capacity */
    return dynarray_info(a)->capacity;
}

void dynarray_setlen(void* a, size_t len) {
    /* change length of array to len,
     * allocating additional memory if necessery */
    dynarray_setcap(a, len);
    dynarray_info(a)->length = len;
}

void* dynarray_setcap(void* a, size_t cap) {
    /* simply ensure array have capacity of 
     * at least cap */
    if (cap <= dynarray_cap(a))
        return a;
    void *b = ((dynarray_info_t*)
            realloc(dynarray_info(a), sizeof(dynarray_info_t) +
            cap * dynarray_membsize(a)))+1;
    dynarray_info(b)->capacity = cap;
    return b;
}

void* _dynarray_new(size_t membsize) {
    /* initialize a new dynarray */
    dynarray_info_t* a;
    a = malloc(sizeof(dynarray_info_t) + membsize * DYNARRAY_MIN_CAPACITY);
    a->length = 0;
    a->capacity = 4; //DYNARRAY_MIN_CAPACITY;
    a->membsize = membsize;
    return a+1;
}

#endif /* SHEEP_DYNARRAY_IMPLEMENTATION */
