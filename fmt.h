/* fmt.h - Customizable printf alternative
 * 
 * Usage: see test/fmt.c
 * Note: this is experimental and I am not sure how portable this is!
 * use at your own risk
 * Author: sleepntsheep
 */
#ifndef SHEEP_FMT_H
#define SHEEP_FMT_H

#include <stdbool.h>
#include <stdio.h>

#define fmt(...) ffmt(stdout, __VA_ARGS__)
#define efmt(...) ffmt(stderr, __VA_ARGS__)

typedef void (*fmt_callback)(FILE *, const void *const arg);

void fmt_register(const char *, fmt_callback);
void ffmt(FILE *out, const char *, ...);
void fmt_set_flush(bool set);

#endif /* SHEEP_FMT_H */

#ifdef SHEEP_FMT_IMPLEMENTATION
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static struct {
    const char *k;
    fmt_callback f;
    size_t l;
} *custom_fmts = NULL;
static size_t custom_fmts_p = 0;
static bool fmt_flush = false;

void fmt_set_flush(bool set) {
    fmt_flush = set;
}

void fmt_register(const char *key, fmt_callback callback) {
    if (custom_fmts == NULL) custom_fmts = malloc(sizeof *custom_fmts);
    else custom_fmts = realloc(custom_fmts, sizeof *custom_fmts * (custom_fmts_p + 1));
    custom_fmts[custom_fmts_p].k = key;
    custom_fmts[custom_fmts_p].f = callback;
    custom_fmts[custom_fmts_p].l = strlen(key);
    custom_fmts_p++;
}

void ffmt(FILE *out, const char *fstr, ...) {
    va_list argptr;
    va_start(argptr, fstr);
    const char *f = fstr;
    while (*f) {
        if (*f != '{' || strchr(f, '}') == NULL) {
            /* could be faster, but IO bottleneck it anyway so who care */
            fputc(*f, out);
            f++;
        } else {
            f++;
            if (strncmp(f, "int}", 4) == 0) {
                fprintf(out, "%d", va_arg(argptr, int));
                f += 4;
            } else if (strncmp(f, "uint}", 5) == 0) {
                fprintf(out, "%u", va_arg(argptr, unsigned int));
                f += 5;
            } else if (strncmp(f, "bool}", 5) == 0) {
                fputs(va_arg(argptr, int) ? "true": "false", out);
                f += 5;
            } else if (strncmp(f, "char}", 5) == 0) {
                fprintf(out, "%c", va_arg(argptr, int));
                f += 5;
            } else if (strncmp(f, "short}", 6) == 0) {
                fprintf(out, "%hd", va_arg(argptr, int));
                f += 6;
            } else if (strncmp(f, "ushort}", 7) == 0) {
                fprintf(out, "%hd", va_arg(argptr, int));
                f += 7;
            } else if (strncmp(f, "long}", 5) == 0) {
                fprintf(out, "%ld", va_arg(argptr, long));
                f += 5;
            } else if (strncmp(f, "ulong}", 6) == 0) {
                fprintf(out, "%lu", va_arg(argptr, long));
                f += 6;
            } else if (strncmp(f, "longlong}", 9) == 0) {
                fprintf(out, "%lld", va_arg(argptr, long long));
                f += 9;
            } else if (strncmp(f, "ulonglong}", 10) == 0) {
                fprintf(out, "%llu", va_arg(argptr, unsigned long long));
                f += 10;
            } else if (strncmp(f, "ptr}", 4) == 0) {
                fprintf(out, "%p", va_arg(argptr, const void *const));
                f += 4;
            } else if (strncmp(f, "str}", 4) == 0) {
                fprintf(out, "%s", va_arg(argptr, char *));
                f += 4;
            } else if (strncmp(f, "float}", 6) == 0) {
                fprintf(out, "%f", va_arg(argptr, double));
                f += 6;
            } else if (strncmp(f, "double}", 7) == 0) {
                fprintf(out, "%f", va_arg(argptr, double));
                f += 7;
            } else if (strncmp(f, "longdouble}", 11) == 0) {
                fprintf(out, "%Lf", va_arg(argptr, long double));
                f += 11;
            } else {
                int found = 0;
                for (size_t i = 0; i < custom_fmts_p; i++) {
                    if (strncmp(f, custom_fmts[i].k, custom_fmts[i].l) == 0
                            && f[custom_fmts[i].l] == '}') {
                        custom_fmts[i].f(out, va_arg(argptr, const void *const));
                        f += custom_fmts[i].l + 1;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    fputc('{', out);
                }
            }
        }
    }
    va_end(argptr);
    if (fmt_flush) fflush(out);
}

#undef SHEEP_FMT_IMPLEMENTATION
#endif /* SHEEP_FMT_IMPLEMENTATION */

