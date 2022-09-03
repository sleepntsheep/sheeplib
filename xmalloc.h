/* xmalloc.h - sleepntsheep 2022
 * need to be used with log.h
 * xmalloc may be considered harmful
 * but sometimes it's useful
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
#include "log.h"

void *check_alloc(void *ptr) {
    if (ptr == NULL)
        panic("allocation failed");
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

#endif /* SHEEP_XMALLOC_IMPLEMENTATION */
