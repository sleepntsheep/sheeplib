/* algo.h
 *
 * SALGO_DECL(type, threewaycomparefunction)
 *   - generate all type-safe algorithm functions 
 *     for "type" type.
 *     threewaycomparefunction is a function
 *     taking type "type" parameters - a, b 
 *     which return negative number if a < b 
 *     zero if a == b and positive number if a > b
 *
 * ssort_type(*base, nmemb) 
 *   - sorting algo
 *
 * sbsearch_type(key, *base, nmemb)
 *   - binary search
 * 
 * slsearch_type(key, *base, nmemb)
 *   - linear search, base doesn't need to be sorted
 *
 * supperbound_type(key, *base, nmemb)
 *   - std::upperbound like function, search for first element in array
 *     that is greater than key
 *
 * slowerbound_type(key, *base, nmemb)
 *   - std::lowerbound like function, search for first element in array
 *     that is lesser or equal to key
 * 
 * Notes: 
 *    ssort must be O(nlogn) average.
 *    sbsearch, supperbound and slowerbound must be O(logn) max.
 *
 * sleepntsheep 2022
 */

#pragma once

#ifndef SHEEP_ALGO_H
#define SHEEP_ALGO_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define SALGO_DECL(type,cmpfnc)\
    void ssort__##type(type *base, size_t nmemb);\
    type *sbsearch_##type(type key, type *base, size_t nmemb);\
    type *supperbound_##type(type key, type *base, size_t nmemb);\
    type *slowerbound_##type(type key, type *base, size_t nmemb);\
    type *slsearch_##type(type key, type *base, size_t nmemb);\
\
    void ssort_merge_##type(type *base, size_t left,\
            size_t mid, size_t right) {\
        size_t ln = mid - left + 1;\
        size_t rn = right - mid;\
\
        type *ltemp = malloc(ln * sizeof(type));\
        type *rtemp = malloc(rn * sizeof(type));\
\
        size_t li, ri, put, i;\
\
        for (i = 0; i < ln; i++)\
            ltemp[i] = base[left+i];\
        for (i = 0; i < rn; i++)\
            rtemp[i] = base[mid+1+i];\
        memcpy(ltemp, base+left, ln*sizeof(type));\
        memcpy(rtemp, base+mid+1, rn*sizeof(type));\
\
        put = left;\
        li = ri = 0;\
\
        for (;li < ln && ri < rn; put++)\
            if (cmpfnc(ltemp[li], rtemp[ri]) <= 0)\
                base[put] = ltemp[li++];\
            else\
                base[put] = rtemp[ri++];\
\
        for (;li < ln; put++)\
            base[put] = ltemp[li++];\
\
        for (;ri < rn; put++)\
            base[put] = rtemp[ri++];\
\
        free(ltemp);\
        free(rtemp);\
    }\
\
    void ssort_helper_##type(type *base, size_t left, size_t right) {\
        size_t mid;\
        if (right - left < 1)\
            return;\
        mid = left + (right - left) / 2;\
        ssort_helper_##type(base, left, mid);\
        ssort_helper_##type(base, mid+1, right);\
        ssort_merge_##type(base, left, mid, right);\
    }\
\
    void ssort_##type(type *base, size_t nmemb) {\
        ssort_helper_##type(base, 0, nmemb - 1);\
    }\
\
    type *sbsearch_##type(type key, type *base, size_t nmemb) {\
        long l, r;\
        int cmp;\
        l = 0;\
        r = nmemb - 1;\
        while (l <= r) {\
            long m = l + (r-l) / 2;\
            cmp = cmpfnc(base[m], key);\
            if (cmp < 0) {\
                l = m + 1;\
            } else if (cmp > 0) {\
                r = m - 1;\
            } else {\
                return base + m;\
            }\
        }\
        return NULL;\
    }\
\
    type *supperbound_##type(type key, type *base, size_t nmemb) {\
        long l, r;\
        int cmp;\
        type *result = NULL;\
        l = 0;\
        r = nmemb - 1;\
        while (l <= r) {\
            long m = l + (r-l) / 2;\
            cmp = cmpfnc(base[m], key);\
            if (cmp <= 0) {\
                l = m + 1;\
            } else if (cmp > 0) {\
                result = base + m;\
                r = m - 1;\
            }\
        }\
        return result;\
    }\
\
    type *slowerbound_##type(type key, type *base, size_t nmemb) {\
        long l, r;\
        int cmp;\
        type *result = NULL;\
        l = 0;\
        r = nmemb - 1;\
        while (l <= r) {\
            long m = l + (r-l) / 2;\
            cmp = cmpfnc(base[m], key);\
            if (cmp < 0) {\
                l = m + 1;\
            } else if (cmp >= 0) {\
                result = base + m;\
                r = m - 1;\
            }\
        }\
        return result;\
    }\
\
    type *slsearch_##type(type key, type *base, size_t nmemb) {\
        size_t i = 0;\
        for (i = 0; i < nmemb; i++)\
            if (cmpfnc(base[i], key) == 0)\
                return base + i;\
        return NULL;\
    }

#endif /* SHEEP_ALGO_H */
