#if !defined(con_t) || !defined(val_t)
#error "Define con_t and val_t before including"
#endif

/* preprocessor magic */
#define Ccat3(a,b) a##_##b
#define Ccat2(a,b) Ccat3(a,b)
#define Cpref(x) Ccat2(con_t,x)

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct
{
    size_t length;
    size_t alloc;
    val_t *data;
} con_t;

con_t Cpref(init)(void)
{
    con_t v;
    v.length = 0;
    v.alloc = 0;
    v.data = NULL;
    return v;
}

void Cpref(grow)(con_t *v, size_t n)
{
    if (v->length + n >= v->alloc)
    {
        v->alloc = v->alloc ? v->alloc * 2 : 32;
        if (v->alloc < v->length + n)
            v->alloc = v->length + n;
        v->data = (val_t*)realloc(v->data, sizeof(v->data[0]) * v->alloc);
    }
}

void Cpref(push)(con_t *v, val_t x)
{
    Cpref(grow)(v, 1);
    v->data[v->length++] = x;
}

val_t Cpref(top)(con_t *v)
{
    assert(v->length);
    return v->data[v->length - 1];
}

void Cpref(pop)(con_t *v)
{
    if (v->length == 0) return;
    v->length--;
}

void Cpref(erase)(con_t *v, size_t i)
{
    assert(v->length > i);
    memmove(v->data + i, v->data + i + 1,
            sizeof(v->data[0]) * (v->length - 1 - i));
    v->length--;
}

void Cpref(insert)(con_t *v, size_t i, val_t x)
{
    Cpref(grow)(v, 1);
    assert(v->length > i);
    memmove(v->data + i + 1, v->data + i,
            sizeof(v->data[0]) * (v->length - i));
    v->data[i] = x;
    v->length++;
}

void Cpref(clear)(con_t *v)
{
    v->length = 0;
}

size_t Cpref(size)(con_t *v)
{
    return v->length;
}

void Cpref(free)(con_t *v)
{
    free(v->data);
}

#undef Ccat3
#undef Ccat2
#undef Cpref
#undef con_t
#undef val_t

