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
#define panic(...) do {  \
                        __stderr_log("PANIC", __FL__, __VA_ARGS__);     \
                        exit(1);                \
                    } while (0)
#define panicerr(...) do {  \
                        __stderr_log("PANIC", __FL__, __VA_ARGS__);     \
                        perror("");             \
                        exit(1);                \
                    } while (0)
#define warn(...) do {  \
                        __stderr_log("WARN", __FL__, __VA_ARGS__);     \
                    } while (0)
#define warnerr(...) do {  \
                        __stderr_log("WARN", __FL__, __VA_ARGS__);     \
                        perror("");             \
                    } while (0)
#define info(...) do {  \
                        __stderr_log("INFO", __FL__, __VA_ARGS__);     \
                    } while (0)
#define infoerr(...) do {  \
                        __stderr_log("INFO", __FL__, __VA_ARGS__);     \
                        perror("");             \
                    } while (0)

void
__stderr_log(const char *type, const char *file,
        const int line, const char *fmt, ...);

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


#endif /* SHEEP_LOG_IMPLEMENTATION */

