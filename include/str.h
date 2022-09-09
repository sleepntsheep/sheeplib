/* str.h - v0.01 - string library for C
 *
 * str_new()
 *   - create & return new empty str
 * 
 * str_from_c(char *p)
 *   - take in char* string and return str with same content
 *     notes: this does not clone the char array
 *
 * str_from_cn(char *p, size_t n)
 *   - make a new str containing first n bytes of p
 *
 * str_from_copy_c(char *p)
 *   - take in char* string and return str with same content
 *     notes: this clone the char array hence "copy"
 *
 * str_cat(str *s, str s2)
 *   - take in pointer of first str and concat second str to it
 *      returning the pointer of concat-ed string
 *
 * str_cat_c(str *s, char *p)
 *   - basically str_cat_str(s, str_from_c(p))
 *
 * str_dup(str s)
 *   - allocate str and copy the content of s to it
 *     return pointer to the the new string 
 *     notes: this * does not * copy the char array buffer
 *     to copy buffer too, use str_substr(s, 0, s.l-1)
 *  
 * str_resize(str *s, size_t size)
 *   - take pointer of str s, 
 *     resize str s to size, returning same pointer
 *     work even if buffer is NULL
 * 
 * str_cmp(str a, str b)
 *   - like stdlib's strcmp, but doesn't depend on null terminator
 * 
 * str_cmp_c(str a, char *p)
 *   - basically str_cmp(a, str_from_c(p));
 * 
 * str_aprintf(const char *fmt, ...)
 *   - take in printf-style format and variadic arguments
 *     allocate str large enough for it
 *     print the content to str and return it
 *     * while this is similar to GNU's asprintf,
 *     this implementation doesn't require GNU's compiler
 * 
 * str_substr(str s, size_t start, size_t end)
 *   - make a new str containing substr of s,
 *     from index start to end
 *
 * str_find_sub(str haystack, str needle)
 *   - return index to first needle in haystack if found
 *     if not found, return -1
 *
 * str_advance(str *s, size_t n)
 *   - advance buffer pointer by n byte, reducing length by n
 *
 * Notes: 
 *     in order to maintain compatibility with 
 *     other library that use NUL-terminated string,
 *     we have to copy the buffer in some function.
 *     This cause these functions' time complexity to be O(n)
 *     when it shouldn't have been
 *     namely,
 *       - str_substr
 *       - str_from_cn
 *
 * - sleepntsheep 2022
 *
 * TODO :
 *     be independent of string.h
 */

#pragma once
#ifndef SHEEP_STR_H
#define SHEEP_STR_H

#include <stddef.h>
#include <stdio.h>

typedef struct str str;
struct str
{
	char* b; /* buffer */
	size_t c, l; /* capacity, length */
};

typedef struct strarray strarray;
struct strarray
{
    struct str *a;
    size_t c, l;
};

struct str  str_new();
struct str *str_new_alloc();
struct str  str_from_c(char* p);
struct str  str_from_cn(char *p, size_t n);
struct str  str_from_copy_c(char *p);
struct str  str_substr(struct str str, size_t start, size_t end);
int         str_cmp(struct str a, struct str b);
size_t      str_find_sub(struct str haystack, struct str needle);
struct str *str_dup(struct str s);
void        str_cat(struct str* s, struct str n);
void        str_resize(struct str* s, size_t newsz);
void        str_advance(struct str* s, size_t n);
struct str *str_aprintf(const char* fmt, ...);

#define str_cmpc(a, p) str_cmp(a, str_from_copy_c(p))
#define str_catc(a, p) str_cat(a, str_from_copy_c(p))
#define str_find_subc(a, p) str_find_sub(a, str_from_copy_c(p))

#define SHEEP_STR_INIT_CAP 512

struct strarray str_split(struct str s, struct str delim);
#define str_splitc(s, delim) str_split(s, str_from_copy_c(delim))

struct strarray strarray_new();
void strarray_push(struct strarray *a, struct str s);

#endif /* SHEEP_STR_H */

#ifdef SHEEP_STR_IMPLEMENTATION
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#ifndef sheep_strlen
size_t sheep_strlen(char *s) {
    size_t i = 0;
    while (s[i])
        i++;
    return i;
}
#endif

#ifndef sheep_strcpy
char* sheep_strcpy(char *dest, const char *src) {
	size_t i = 0;
	while ((dest[i] = src[i]))
		i++;
    return dest;
}
#endif

struct str str_new()
{
	char* b = calloc(SHEEP_STR_INIT_CAP, 1);
	return (struct str){ b, SHEEP_STR_INIT_CAP, 0 } ;
}

struct str *str_new_alloc()
{
    return str_dup(str_new());
}

struct str str_from_c(char* p)
{
    size_t len = sheep_strlen(p);
	struct str s = { 0 }; 
	if (p == NULL)
		return s;
    s.c = len + 1;
    s.l = s.c = len;
    s.b = p;
	return s;
}

struct str str_from_cn(char *p, size_t n)
{
    size_t len = sheep_strlen(p);
    size_t i;
    if (n < len) len = n;
    struct str s = str_new();
	if (p == NULL) return s;
    str_resize(&s, s.c = len + 1);
    s.l = len;
    for (i = 0; i < len; i++)
        s.b[i] = p[i];
    return s;
}

struct str str_from_copy_c(char *p)
{
    struct str s = { 0 };
	if (p == NULL)
		return s;
    s.l = sheep_strlen(p);
    s.c = s.l + 1;
    str_resize(&s, s.c);
    sheep_strcpy(s.b, p);
    return s;
}

struct str str_substr(struct str str, size_t start, size_t end)
{
    /* We can't get rid of NUL-terminated string, sadly
     * many library depend on that and if we do we will have
     * to implement too much thing like printf replacement */
    struct str substring = { 0 };
    substring.l = end - start + 1;
    substring.c = substring.l + 1;
    substring.b = malloc(substring.c);
    sheep_strcpy(substring.b, str.b + start);
    substring.b[substring.l] = '\x0';
    return substring;
}

int str_cmp(struct str a, struct str b)
{
    if (a.b == NULL && b.b == NULL)
        return 0;
    size_t i;
    if (a.l < b.l)
        return -1;
    else if (a.l > b.l)
        return 1;
    for (i = 0; i < a.l; i++)
        if (a.b[i] != b.b[i])
            return a.b[i] - b.b[i];
    return 0;
}

void str_cat(struct str *s, struct str n)
{
	if (s->l + n.l + 1 >= s->c) {
		str_resize(s, s->l + n.l + 1);
    }
	sheep_strcpy(s->b + s->l, n.b);
	s->l += n.l;
}

size_t str_find_sub(struct str haystack, struct str needle)
{
    /* kmp algo */
    size_t m = 0;
    if (haystack.l < needle.l) return -1;

    while (m <= haystack.l - needle.l) {
        int ismatch = 1;
        size_t j;
        for (j = 0; j < needle.l; j++) {
            if (haystack.b[m+j] != needle.b[j]) {
                ismatch = 0;
                break;
            }
        }
        if (ismatch) {
            return m;
        }
        m += j + 1;
    }
    return -1;
}

void str_resize(struct str* s, size_t newsz)
{
    if (s->b == NULL)
        s->b = malloc(newsz);
    else if (newsz > s->c) {
		s->b = realloc(s->b, newsz);
        s->b[newsz] = '\0';
    }
    s->c = newsz;
}

size_t
__sprintf_sz(const char* fmt, ...)
{
	size_t len;
	va_list args;
	va_start(args, fmt);
	len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);
	return len;
}

struct str*
str_aprintf(const char* fmt, ...)
{
    size_t len;
	va_list args;
	va_start(args, fmt);
	len = __sprintf_sz(fmt, args);
	va_end(args);
	struct str *s = str_new_alloc();
    str_resize(s, len);
	va_start(args, fmt);
	vsnprintf(s->b, len, fmt, args);
	va_end(args);
    s->l = sheep_strlen(s->b);
	return s;
}

struct str*
str_dup(struct str s)
{
    struct str* ret = calloc(1, sizeof(*ret));
    ret->c = s.c;
    ret->l = s.l;
    ret->b = s.b;
    return ret;
}

struct strarray str_split(struct str s, struct str delim)
{
    size_t length = 0, capacity = 4;
    size_t i, lastdelim;
    struct strarray arr;
    arr = strarray_new();
	for (;;)
	{
        i = str_find_sub(s, delim);
        if (i == -1) break;
        strarray_push(&arr, str_substr(s, 0, i-1));
        str_advance(&s, i + delim.l);
	}
    strarray_push(&arr, str_substr(s, 0, s.l-1));
	return arr;
}

void str_advance(struct str* s, size_t n) {
    if (n > s->l)
        return;
    s->b += n;
    s->l -= n;
}

struct strarray strarray_new() {
    struct strarray a;
    a.c = 4;
    a.l = 0;
    a.a = malloc(sizeof(struct str) * a.c);
    return a;
}

void strarray_push(struct strarray *a, struct str s) {
    if (a->l == a->c)
        a->a = realloc(a->a, (a->c *= 2) * sizeof(struct str));
    a->a[a->l++] = s;
}

#endif /* SHEEP_STR_IMPLEMENTATION */
