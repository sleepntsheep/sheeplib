/** @file xmalloc.h 
 * @brief x-wrapper for malloc, realloc, calloc, free
 *
 * - sleepntsheep 2022
 * */

#pragma once
#ifndef SHEEP_XMALLOC_H
#define SHEEP_XMALLOC_H

#include <stddef.h>

void *check_alloc(void *ptr);

void *xmalloc(size_t size);
void *xcalloc(size_t nmemb, size_t size);
void *xrealloc(void *ptr, size_t size);

#endif /* SHEEP_XMALLOC_H */

#ifdef SHEEP_XMALLOC_IMPLEMENTATION

#include <stdlib.h>

void *check_alloc(void *ptr) {
    if (ptr == NULL)
        abort();
    return ptr;
}

void *xmalloc(size_t size) {
    return check_alloc(malloc(size));
}

void *xcalloc(size_t nmemb, size_t size) {
    return check_alloc(calloc(nmemb, size));
}

void *xrealloc(void *ptr, size_t size) {
    return check_alloc(realloc(ptr, size));
}

void xfree(void *ptr) {
    if (ptr != NULL)
        free(ptr);
}

#endif /* SHEEP_XMALLOC_IMPLEMENTATION */
