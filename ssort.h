/* ssort.h
 *
 * ssort | sheep_qsort - stdlib's qsort replacement
 *
 * sbsearch | sheep_bsearch - stdlib's bsearch replacement
 *
 * supperbound | sheep_upperbound 
 *  - std::upperbound like function, search for first element in array
 *    that is greater than key
 *
 * slowerbound | sheep_lowerbound 
 *  - std::lowerbound like function, search for first element in array
 *    that is greater or equal to key
 * 
 * Notes: 
 *    ssort must be O(nlogn) average.
 *    sbsearch, supperbound and slowerbound must be O(logn) max.
 *
 * sleepntsheep 2022
 */

#ifndef SHEEP_SSORT_H
#define SHEEP_SSORT_H
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void sheep_qsort(void *base, size_t nmemb,
				size_t size, int (*cmpfnc)(const void *, const void *));

void *sheep_bsearch(void *key, void *base, size_t nmemb,
        size_t size, int (*cmpfnc)(const void *, const void *));

void *sheep_upperbound(void *key, void *base, size_t nmemb,
        size_t size, int (*cmpfnc)(const void *, const void *));

void *sheep_lowerbound(void *key, void *base, size_t nmemb,
        size_t size, int (*cmpfnc)(const void *, const void *));

#define ssort sheep_qsort
#define sbsearch sheep_bsearch
#define supperbound sheep_upperbound
#define slowerbound sheep_lowerbound

#endif /* SHEEP_SSORT_H */

#ifdef SHEEP_SSORT_IMPLEMENTATION

void sheep_qsort_merge(char *base, size_t left,
        size_t mid, size_t right, size_t size,
        int (*cmpfnc)(const void*, const void*)) {
    size_t lnmemb = mid - left + 1;
    size_t rnmemb = right - mid;

    /* having ltemp and rtemp as function parameter 
     * or global variable do make this faster, 
     * but not by a lot. Only important thing here is
     * Big-O notation which must be O(nlogn) */
    char *ltemp = malloc(lnmemb * size);
    char *rtemp = malloc(rnmemb * size);

    size_t li, ri, put;

    memcpy(ltemp, base + left * size, lnmemb * size);
    memcpy(rtemp, base + (mid + 1) * size, rnmemb * size);

    li = ri = put = 0;

    while (li < lnmemb && ri < rnmemb)
        if (cmpfnc(ltemp + li * size, rtemp + ri * size) <= 0)
            memcpy(base + (left + put++) * size, ltemp + li++ * size, size);
        else
            memcpy(base + (left + put++) * size, rtemp + ri++ * size, size);

    while (li < lnmemb)
        memcpy(base + (left + put++) * size, ltemp + li++ * size, size);

    while (ri < rnmemb)
        memcpy(base + (left + put++) * size, rtemp + ri++ * size, size);

    free(ltemp);
    free(rtemp);
}

void sheep_qsort_helper(void *base, size_t left, size_t right,
        size_t size, int (*cmpfnc)(const void *, const void *)) {
    size_t mid;
    if (right - left < 1)
        return;
    mid = left + (right - left) / 2;
    sheep_qsort_helper(base, left, mid, size, cmpfnc);
    sheep_qsort_helper(base, mid+1, right, size, cmpfnc);
    sheep_qsort_merge(base, left, mid, right, size, cmpfnc);
}

void sheep_qsort(void *base, size_t nmemb,
        size_t size, int (*cmpfnc)(const void *, const void *)) {
    sheep_qsort_helper(base, 0, nmemb - 1, size, cmpfnc);
}

void *sheep_bsearch(void *key, void *base, size_t nmemb,
        size_t size, int (*cmpfnc)(const void *, const void *)) {
    size_t l, r;
    int cmp;
    l = 0;
    r = nmemb - 1;
    while (l <= r) {
        size_t m = l + (r - l) / 2;
        void *cur = (char*) base + m * size;
        cmp = cmpfnc(cur, key);
        if (cmp < 0)
            l = m + 1;
        else if (cmp > 0)
            r = m - 1;
        else
            return cur;
    }
    return NULL; 
}

void *sheep_upperbound(void *key, void *base, size_t nmemb,
        size_t size, int (*cmpfnc)(const void *, const void *)) {
    size_t l, r;
    int cmp;
    void *result = NULL;
    l = 0;
    r = nmemb - 1;
    while (l <= r) {
        size_t m = l + (r - l) / 2;
        void *cur = (char*) base + m * size;
        cmp = cmpfnc(cur, key);
        if (cmp <= 0) {
            l = m + 1;
        } else if (cmp > 0) {
            result = cur;
            r = m - 1;
        }
    }
    return result;
}

void *sheep_lowerbound(void *key, void *base, size_t nmemb,
        size_t size, int (*cmpfnc)(const void *, const void *)) {
    size_t l, r;
    int cmp;
    void *result = NULL;
    l = 0;
    r = nmemb - 1;
    while (l <= r) {
        size_t m = l + (r - l) / 2;
        void *cur = (char*) base + m * size;
        cmp = cmpfnc(cur, key);
        if (cmp < 0) {
            l = m + 1;
        } else if (cmp >= 0) {
            result = cur;
            r = m - 1;
        }
    }
    return result;
}

#endif /* SHEEP_SSORT_IMPLEMENTATION */
