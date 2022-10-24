/* profiler.h
 * Simple single header profiler for C
 *
 * Usage:
 *
 *  // make a context (must be zero-initialized)
 *  struct profiler pf = { 0 };
 *
 *  profiler_start(&pf, "IO Op");
 *  FILE *fp = fopen("/dev/null", "w");
 *  fprintf(fp, "Profiler!");
 *  fclose(fp);
 *  profiler_stop(&pf, "IO Op");
 *  
 *  // print profiled data
 *
 *  profiler_print(&pf);
 *  // must cleanup to prevent memory leak
 *  profiler_cleanup(&pf);
 */

#ifndef SHEEP_PROFILER_H
#define SHEEP_PROFILER_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>

struct profile {
    char *name;
    clock_t total;
    clock_t start;
};

struct profiler {
    struct profile *profiles;
    size_t len;
};

void profiler_init(struct profiler *profiler);
void profiler_cleanup(struct profiler *profiler);
void profiler_start(struct profiler *profiler, const char *name);
void profiler_stop(struct profiler *profiler, const char *name);

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

static struct profile *_profiler_find(struct profiler *profiler, const char *name)
{
    struct profile *target = NULL;
    profiler_foreach(prof, i, profiler, {
        if (strcmp(name, prof->name) == 0)
        {
            target = prof;
        }
    });
    return target;
}

static struct profile *_profiler_find_create(struct profiler *profiler, const char *name)
{
    if (profiler->profiles == NULL)
    {
        profiler->len = 0;
        profiler->profiles = malloc(sizeof *profiler->profiles);
    }
    struct profile *target = _profiler_find(profiler, name);
    if (target)
    {
        return target;
    }
    else
    {
        profiler->profiles = realloc(profiler->profiles, sizeof *profiler->profiles * (profiler->len + 1));
        profiler->profiles[profiler->len++] = (struct profile) {
            .name = _profiler_strdup(name),
        };
        return profiler->profiles + profiler->len - 1;
    }
}

void profiler_cleanup(struct profiler *profiler)
{
    if (profiler->profiles == NULL)
        return;
    profiler_foreach(prof, i, profiler, {
        free(prof->name);
    })
    free(profiler->profiles);
}

void profiler_start(struct profiler *profiler, const char *name)
{
    struct profile *target = _profiler_find_create(profiler, name);
    _profile_start(target);
}

void profiler_stop(struct profiler *profiler, const char *name)
{
    struct profile *target = _profiler_find_create(profiler, name);
    _profile_stop(target);
}

void profiler_print(struct profiler *profiler)
{
    if (profiler->profiles == NULL)
        return;
    int max_name_len = 0;
    uintmax_t total_clock = 0;
    int max_clock_len = 0;
    profiler_foreach(prof, i, profiler, {
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
    profiler_foreach(prof, i, profiler, {
        printf("%*s: %*.0lf -- %2.2lf%\n", max_name_len, prof->name, max_clock_len,
                (double)prof->total, 100 * prof->total / (double)total_clock);
    })
}

#endif /* SHEEP_PROFILER_IMPLEMENTATION */

