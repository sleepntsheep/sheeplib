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

#define str_cat(a,b) \
	_Generic((b), \
		struct str: str_cat_str, \
		default: str_cat_cstr \
	)(a,b)

struct str str_new();
struct str cstr(char* p);
struct str str_cat_str(struct str* s, struct str n);
struct str* str_dup(struct str s);
struct str str_cat_cstr(struct str* s, char* n);
void str_free(struct str* s);
struct str str_resize(struct str* s, size_t newsz);
struct str str_aprintf(const char* fmt, ...);

#ifdef SHEEP_STR_HAVE_STB_DS

#define str_split(a,b) \
	_Generic((b), \
		struct str: str_split_str, \
		default: str_split_cstr \
	)(a,b)
struct str* str_split_str(struct str haystack, struct str needle);
struct str* str_split_cstr(struct str haystack, char* needle);

#endif /* SHEEP_STR_HAVE_STB_DS */

#endif /* SHEEP_STR_H */

#ifdef SHEEP_STR_IMPLEMENTATION
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

struct str
str_new()
{
	char* b = calloc(512, 1);
	return (struct str){ b, 512, 0 } ;
}

struct str
cstr(char* p)
{
	struct str s = str_new();
	if (p == NULL)
		return s;
	size_t len = strlen(p);
	s = str_resize(&s, len+1);
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

struct str
	str_npush(struct str s,
		char* n, size_t len)
{
	if (s.l + len >= s.c)
		str_resize(&s, s.l + len + 1);
	strncpy(s.b + s.l, n, len);
	s.b[s.l] = 0;
	s.l += len;
	return s;
}

#ifdef SHEEP_STR_HAVE_STB_DS 

struct str*
str_split_str(struct str haystack,
	struct str needle)
{
	struct str* arr = NULL;
	long off = 0;
	long linelen = 0;
	for (;;)
	{
		char* sp = haystack.b + off;
		char* ch = strstr(sp, needle.b);
		if (ch == NULL)
			break;
		linelen = ch - sp;
		struct str line = str_npush(str_new(), sp, linelen);
        str_cat(&line, ""); // nil terminator
		arrput(arr, line);
		off += linelen + needle.l;
	}
	return arr;
}

struct str* str_split_cstr(struct str haystack, char* needle)
{
	return str_split_str(haystack, cstr(needle));
}

#endif /* SHEEP_STR_HAVE_STB_DS */

void str_free(struct str* s)
{
	free(s->b);
	free(s);
}

struct str
str_resize(struct str* s, size_t newsz)
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
	return *s;
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

struct str
str_aprintf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t len = __sprintf_sz(fmt, args);
	va_end(args);
	struct str s = str_resize(str_dup(str_new()), len);
	va_start(args, fmt);
	vsnprintf(s.b, len, fmt, args);
	va_end(args);
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
