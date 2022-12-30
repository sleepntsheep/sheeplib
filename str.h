#ifndef SHEEP_STR_H
#define SHEEP_STR_H

#include <stddef.h>

#ifndef STR_MALLOC
#include <stdlib.h>
#define STR_MALLOC malloc
#endif

#ifndef STR_MEMCPY
#include <string.h>
#define STR_MEMCPY memcpy
#endif

struct _str {
    size_t length;
    size_t alloc;
    char a[1];
};

typedef char* str;

str str_new(void);
size_t str_length(str s);
size_t str_alloc(str s);
str str_from(const char *cstr);
str str_cat(str s, const char *cstr);
str str_realloc(str s, size_t newalloc);
str str_format(const char *fmt, ...);
void str_free(str s);

#endif /* SHEEP_STR_H */

#ifdef SHEEP_STR_IMPLEMENTATION

#include <stdarg.h>

#define SHEEP_STR_INIT_ALLOC 64

str str_new(void)
{
    struct _str *ss;
    ss = STR_MALLOC(offsetof(struct _str, a) + SHEEP_STR_INIT_ALLOC);
    ss->alloc = SHEEP_STR_INIT_ALLOC;
    ss->length = 0;
    return ss->a;
}

static struct _str *_str_from_str(str s)
{
    return (struct _str*)(s - offsetof(struct _str, a));
}

size_t str_length(str s)
{
    struct _str *ss = _str_from_str(s);
    return ss->length;
}

size_t str_alloc(str s)
{
    struct _str *ss = _str_from_str(s);
    return ss->alloc;
}

str str_from(const char *cstr)
{
    return str_cat(str_new(), cstr);
}

str str_realloc(str s, size_t newalloc)
{
    struct _str *ss = _str_from_str(s);
    ss = realloc(ss, offsetof(struct _str, a) + newalloc);
    ss->alloc = newalloc;
    return ss->a;
}

str str_cat(str s, const char *cstr)
{
    size_t cslen = strlen(cstr);
    if (cslen + str_length(s) > str_alloc(s))
    {
        size_t newalloc = str_alloc(s);
        while (cslen + str_length(s) > newalloc)
            newalloc *= 2;
        s = str_realloc(s, newalloc);
    }
    struct _str *ss = _str_from_str(s);
    STR_MEMCPY(ss->a + ss->length, cstr, cslen);
    ss->length += cslen;
    ss->a[ss->length] = 0;
    return s;
}

str str_format(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    str s = str_new();
    s = str_realloc(s, len + 1);
    va_start(args, fmt);
    vsnprintf(s, len + 1, fmt, args);
    va_end(args);
    return s;
}

void str_free(str s)
{
    free(_str_from_str(s));
}

#endif /* SHEEP_STR_IMPLEMENTATION */


