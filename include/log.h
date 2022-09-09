/* log.h - simple logging library - sleepntsheep 2022
 * idea is that logging should be simple and easy, 
 * here you put in argument just as how you would printf it
 * panic(...) : log at panic level and abort
 * warn(...) : log at warn level
 * info(...) : log at info level
 * the *err(...) counterpart of each function
 * do the same thing except it call perror at the end 
 * causing error from errno to be printed too
 */

#ifndef SHEEP_LOG_H
#define SHEEP_LOG_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __FL__ __FILE__, __LINE__
#define panic(...) _panic(__FL__, __VA_ARGS__)
#define panicerr(...) _panicerr(__FL__, __VA_ARGS__)
#define warn(...) _warn(__FL__, __VA_ARGS__)
#define warnerr(...) _warnerr(__FL__, __VA_ARGS__)
#define info(...) _info(__FL__, __VA_ARGS__)
#define infoerr(...) _infoerr(__FL__, __VA_ARGS__)

void
__stderr_log(const char *type, const char *file,
        const int line, const char *fmt, ...);

void
_panic(const char *file, const int line,
        const char *fmt, ...);

void
_panicerr(const char *file, const int line,
        const char *fmt, ...);

void
_warn(const char *file, const int line,
        const char *fmt, ...);

void
_warnerr(const char *file, const int line,
        const char *fmt, ...);

void
_info(const char *file, const int line,
        const char *fmt, ...);

void
_infoerr(const char *file, const int line,
        const char *fmt, ...);

#endif /* SHEEP_LOG_H */

#ifdef SHEEP_LOG_IMPLEMENTATION

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#define _sheep_va(x) \
        va_list a; \
        va_start(a, fmt); \
        x; \
        va_end(a)

void
__stderr_log(const char *type, const char *file
        , const int line, const char *fmt
        , ...)
{
    fprintf(stderr, "%s: %s:%d: ", type, file, line);
    _sheep_va(vfprintf(stderr, fmt, a));
    fprintf(stderr, "\n");
    fflush(stderr);
}

void
_panic(const char *file, const int line,
        const char *fmt, ...)
{
    _sheep_va(__stderr_log("panic", file, line, fmt, a));
    exit(EXIT_FAILURE);
}

void
_panicerr(const char *file, const int line,
        const char *fmt, ...)
{
    _sheep_va(__stderr_log("panic", file, line, fmt, a));
    perror("");
    exit(EXIT_FAILURE);
}

void
_warn(const char *file, const int line,
        const char *fmt, ...)
{
    _sheep_va(__stderr_log("warn", file, line, fmt, a));
}

void
_warnerr(const char *file, const int line,
        const char *fmt, ...)
{
    _sheep_va(__stderr_log("warn", file, line, fmt, a));
    perror("");
}

void
_info(const char *file, const int line,
        const char *fmt, ...)
{
    _sheep_va(__stderr_log("info", file, line, fmt, a));
}

void
_infoerr(const char *file, const int line,
        const char *fmt, ...)
{
    _sheep_va(__stderr_log("info", file, line, fmt, a));
    perror("");
}

#endif /* SHEEP_LOG_IMPLEMENTATION */

