/** 
 * @file log.h
 * @brief Logging functions
 */
/* log.h v0.0.1
 * simple logging library - sleepntsheep 2022
 * logging should be simple and easy, 
 * here you put in argument just as how you would printf it
 * panic(...) : log at panic level and abort
 * warn(...) : log at warn level
 * info(...) : log at info level
 * the *err(...) counterpart of each function
 * do the same thing except it call perror at the end 
 * causing error from errno to be printed too
 */

#ifdef __cplusplus
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#else
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#ifndef SHEEP_LOG_H
#define SHEEP_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief log at panic level and abort
 */
#define panic(...) do {  \
                        __stderr_log("PANIC", __FILE__, __LINE__, __VA_ARGS__);     \
                        exit(1);                \
                    } while (0)
/**
 * @brief log at panic level, print errno and abort
 */
#define panicerr(...) do {  \
                        __stderr_log("PANIC", __FILE__, __LINE__, __VA_ARGS__);     \
                        perror("");             \
                        exit(1);                \
                    } while (0)
/**
 * @brief log at warn level
 */
#define warn(...) do {  \
                        __stderr_log("WARN", __FILE__, __LINE__, __VA_ARGS__);     \
                    } while (0)
/**
 * @brief log at warn level, print errno
 */
#define warnerr(...) do {  \
                        __stderr_log("WARN", __FILE__, __LINE__, __VA_ARGS__);     \
                        perror("");             \
                    } while (0)
/**
 * @brief log at info level
 */
#define info(...) do {  \
                        __stderr_log("INFO", __FILE__, __LINE__, __VA_ARGS__);     \
                    } while (0)
/**
 * @brief log at info level, print errno
 */
#define infoerr(...) do {  \
                        __stderr_log("INFO", __FILE__, __LINE__, __VA_ARGS__);     \
                        perror("");             \
                    } while (0)

static void
__stderr_log(const char *type, const char *file,
        const int line, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* SHEEP_LOG_H */

#ifdef SHEEP_LOG_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#include <cstdarg>
#else
#include <stdarg.h>
#endif

static void
__stderr_log(const char *type, const char *file
        , const int line, const char *fmt
        , ...)
{
    fprintf(stderr, "%s: %s:%d: ", type, file, line);
    va_list a;
    va_start(a, fmt);
    vfprintf(stderr, fmt, a);
    va_end(a);
    fprintf(stderr, "\n");
    fflush(stderr);
}

#ifdef __cplusplus
}
#endif

#endif /* SHEEP_LOG_IMPLEMENTATION */

