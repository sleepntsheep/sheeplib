/* profiler.h
 * Simple single header profiler for C
 *
 * Usage:
 *
 *  // make a context (opaque pointer)
 *  Profiler *p = profiler_new();
 *
 *  profiler_start(p, "IO Op");
 *  FILE *fp = fopen("/dev/null", "w");
 *  fprintf(fp, "Profiler!");
 *  fclose(fp);
 *  profiler_stop(p, "IO Op");
 *
 *  // print profiled data
 *  profiler_print(&pf);
 *
 *  // free malloced memory
 *  profiler_free(p);
 *
 */

#ifndef SHEEP_PROFILER_H
#define SHEEP_PROFILER_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>

typedef struct profiler Profiler;

struct profile {
    char *name;
    clock_t total;
    clock_t start;
};

struct profiler {
    struct profile *profiles;
    size_t len;
    struct profile *last;
};

Profiler *profiler_new(void);
void profiler_free(Profiler *p);
void profiler_start(Profiler *p, const char *name);
void profiler_stop(Profiler *p);

#endif /* SHEEP_PROFILER_H */

#ifdef SHEEP_PROFILER_IMPLEMENTATION

#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char *_profiler_strdup(const char *s)
{
    size_t len = strlen(s);
    char *ret = malloc(len + 1);
    if (ret == NULL) return NULL;
    memcpy(ret, s, len);
    ret[len] = 0;
    return ret;
}

#define profiler_foreach(iter, counter, profiler, block) \
    for (size_t counter = 0; counter < (profiler)->len; counter++) \
    { \
        struct profile *iter = (profiler)->profiles + counter; \
        block \
    }

static void _profile_start(struct profile *profile)
{
    profile->start = clock();
}

static void _profile_stop(struct profile *profile)
{
    profile->total += clock() - profile->start;
    profile->start = 0L;
}

static struct profile *_profiler_find(Profiler *p, const char *name)
{
    struct profile *target = NULL;
    profiler_foreach(prof, i, p, {
        if (strcmp(name, prof->name) == 0)
        {
            target = prof;
        }
    });
    return target;
}

static struct profile *_profiler_find_create(Profiler *p, const char *name)
{
    if (p->profiles == NULL)
    {
        p->len = 0;
        p->profiles = malloc(sizeof *p->profiles);
    }
    struct profile *target = _profiler_find(p, name);
    if (target)
    {
        return target;
    }
    else
    {
        p->profiles = realloc(p->profiles, sizeof *p->profiles * (p->len + 1));
        p->profiles[p->len++] = (struct profile) {
            .name = _profiler_strdup(name),
        };
        return p->profiles + p->len - 1;
    }
}

Profiler *profiler_new(void)
{
    Profiler *p = malloc(sizeof *p);
    p->len = 0;
    p->profiles = NULL;
    p->last = NULL;
    return p;
}

void profiler_free(Profiler *p)
{
    if (p->profiles) free(p->profiles);
    free(p);
}

void profiler_start(Profiler *p, const char *name)
{
    struct profile *target = _profiler_find_create(p, name);
    _profile_start(target);
    p->last = target;
}

void profiler_stop(Profiler *p)
{
    if (p->last)
        _profile_stop(p->last);
    /*
    struct profile *target = _profiler_find_create(profiler, name);
    _profile_stop(target);
    */
}

void profiler_print(Profiler *p)
{
    if (p->profiles == NULL)
        return;
    int max_name_len = 0;
    uintmax_t total_clock = 0;
    int max_clock_len = 0;
    profiler_foreach(prof, i, p, {
        int nlen = strlen(prof->name);
        if (max_name_len < nlen) max_name_len = nlen;
        {
            clock_t tmp = prof->total;
            int tlen = 0;
            for (clock_t tmp = prof->total; tmp >= 1; tlen++)
            {
                tmp /= 10;
            }
            if (max_clock_len < tlen) max_clock_len = tlen;
        }
        total_clock += prof->total;
    })
    profiler_foreach(prof, i, p, {
        printf("%*s: %*.0lf -- %2.2lf%\n", max_name_len, prof->name, max_clock_len,
                (double)prof->total, 100 * prof->total / (double)total_clock);
    })
}

#endif /* SHEEP_PROFILER_IMPLEMENTATION */

