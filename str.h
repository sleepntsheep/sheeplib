/* str.h - string library for C
 *
 * str_new()
 *   - create & return new empty str
 * 
 * cstr(char*p)
 *   - take in char* string and return str with same content
 *
 * str_cat_str(str *s, str s2)
 *   - take in pointer of first string and concat second str to it
 *      returning the pointer of concat-ed string
 *
 * str_cat_cstr(str *s, char *p)
 *   - like str_cat_str(s, cstr(p))
 *
 * str_cat(str *s, str | char *)
 *   - _Generic macro that accept both char* and str as 
 *   second parameter, only available in C11 and above
 *
 * str_dup(str s)
 *   - return new allocated str and copy the content of s to it
 *  
 * str_free(str *s)
 *   - free strs allocated by str_dup
 *
 * str_resize(str *s, size_t size)
 *   - take pointer of str s, 
 *     resize str s to size, returning same pointer
 * 
 * - sleepntsheep 2022
 *
 * str_aprintf(const char *fmt, ...)
 *   - take in printf-style format and variadic arguments
 *     allocate str large enough for it
 *     print the content to str and return it
 *     * while this is similar to GNU's asprintf,
 *     this implementation doesn't require GNU's compiler
 */

#pragma once
#ifndef SHEEP_STR_H
#define SHEEP_STR_H

#include <stddef.h>
#include <stdio.h>

typedef struct str str;
struct str
{
	char* b; // buffer
	size_t c, l; // capacity, length
};

struct str str_new();
struct str cstr(char* p);
struct str str_cat_str(struct str* s, struct str n);
struct str* str_dup(struct str s);
struct str str_cat_cstr(struct str* s, char* n);
void str_free(struct str* s);
struct str* str_resize(struct str* s, size_t newsz);
struct str* str_aprintf(const char* fmt, ...);

#define SHEEP_STR_INIT_CAP 512

#if __STDC_VERSION__ >= 201112L
#define str_cat(a,b) \
	_Generic((b), \
		struct str: str_cat_str, \
		default: str_cat_cstr \
	)(a,b)
#endif


#ifdef SHEEP_DYNARRAY_H

#if __STDC_VERSION__ >= 201112L
#define str_split(a,b) \
_Generic((b), \
    struct str: str_split_str, \
    default: str_split_cstr \
)(a,b)
#endif

struct str* str_split_str(struct str haystack, struct str needle);
struct str* str_split_cstr(struct str haystack, char* needle);
#endif /* SHEEP_DYNARRAY_H */

#endif /* SHEEP_STR_H */

#ifdef SHEEP_STR_IMPLEMENTATION
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

struct str
str_new()
{
	char* b = calloc(SHEEP_STR_INIT_CAP, 1);
	return (struct str){ b, SHEEP_STR_INIT_CAP, 0 } ;
}

struct str
cstr(char* p)
{
	struct str s = str_new();
	if (p == NULL)
		return s;
	size_t len = strlen(p);
	str_resize(&s, len+1);
	s.l = len;
	strcpy(s.b, p);
	return s;
}

struct str
str_cat_str(struct str* s,
		struct str n)
{
	if (s->l + n.l >= s->c)
		str_resize(s, s->l + n.l + 1);
	strcpy(s->b + s->l, n.b);
	s->l += n.l;
	return *s;
}

struct str
str_cat_cstr(struct str* s,
		char* n)
{
	return str_cat_str(s, cstr(n));
}

#ifdef SHEEP_DYNARRAY_H

struct str*
str_split_str(struct str haystack,
	struct str needle)
{
	struct str* arr = arrnew(struct str);
	long off = 0;
	long linelen = 0;
	for (;;)
	{
		char* sp = haystack.b + off;
		char* ch = strstr(sp, needle.b);
		if (ch == NULL)
			break;
		linelen = ch - sp;
        sp[linelen] = 0;
		arrpush(arr, cstr(sp));
		off += linelen + needle.l;
	}
	return arr;
}

struct str* str_split_cstr(struct str haystack, char* needle)
{
	return str_split_str(haystack, cstr(needle));
}

#endif /* SHEEP_DYNARRAY_H */

void str_free(struct str* s)
{
	free(s->b);
	free(s);
}

struct str* str_resize(struct str* s, size_t newsz)
{
	if (newsz > s->c)
	{
		s->b = realloc(s->b, s->c = newsz);
	}
	else if (newsz < s->l)
	{
		s->l = newsz;
		s->b[s->l] = 0;
	}
	return s;
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

struct str* str_aprintf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t len = __sprintf_sz(fmt, args);
	va_end(args);
	struct str *s = str_resize(str_dup(str_new()), len);
	va_start(args, fmt);
	vsnprintf(s->b, len, fmt, args);
	va_end(args);
    s->l = strlen(s->b);
	return s;
}

struct str*
str_dup(struct str s)
{
    struct str* ret = calloc(1, sizeof(*ret));
    ret->b = s.b;
    ret->l = s.l;
    ret->c = s.c;
    return ret;
}

#endif /* SHEEP_STR_IMPLEMENTATION */
