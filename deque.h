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
    size_t alloc;
    size_t head;
    size_t tail;
    val_t *data;
} con_t;

con_t Cpref(init)(void)
{
    con_t q;
    q.alloc = 0;
    q.data = NULL;
    q.head = 0;
    q.tail = 0;
    return q;
}

size_t Cpref(size)(con_t *q)
{
    if (q->alloc && (q->head + 1) % q->alloc == q->tail) return q->alloc - 1;
    if (q->head >= q->tail) return q->head - q->tail;
    //if (q->tail - q->head == 1) return q->alloc - 1;
    return q->head + q->alloc - q->tail;
}

size_t Cpref(empty)(con_t *q)
{
    return Cpref(size)(q) == 0;
}

void Cpref(clear)(con_t *q)
{
    q->head = q->tail = 0;
}

void Cpref(grow)(con_t *q, size_t n)
{
    size_t sz = Cpref(size)(q);
    if (sz + n + 1 <= q->alloc) return;
    size_t old_alloc = q->alloc;
    q->alloc = q->alloc ? q->alloc * 2 : 32;
    q->data = (val_t*)realloc(q->data, q->alloc * sizeof(val_t));
    if (q->head >= q->tail)
    {
        memmove(q->data, q->data + q->tail, (q->head - q->tail) * sizeof(val_t));
        q->tail = 0;
        q->head = sz;
    }
    else
    {
        size_t pt = old_alloc - q->tail + 1;
        memcpy(q->data + old_alloc + q->tail, q->data + q->tail, pt * sizeof(val_t));
        q->tail += old_alloc;
    }
}

void Cpref(push_back)(con_t *q, val_t x)
{
    Cpref(grow)(q, 1);
    q->data[q->head] = x;
    if (++q->head == q->alloc) q->head = 0;
}

void Cpref(pop_back)(con_t *q)
{
    assert(Cpref(size)(q));
    if (q->head > 0) --q->head; else q->head = q->alloc - 1;
}

void Cpref(push_front)(con_t *q, val_t x)
{
    Cpref(grow)(q, 1);
    if (q->tail > 0) --q->tail; else q->tail = q->alloc - 1;
    q->data[q->tail] = x;
}

void Cpref(pop_front)(con_t *q)
{
    assert(Cpref(size)(q));
    if (++q->tail == q->alloc) q->tail = 0;
}

val_t Cpref(back)(con_t *q)
{
    assert(Cpref(size)(q));
    return q->head > 0 ? q->data[q->head - 1] : q->data[q->alloc - 1];
}

val_t Cpref(front)(con_t *q)
{
    assert(Cpref(size)(q));
    return q->data[q->tail];
}

#undef Ccat3
#undef Ccat2
#undef Cpref
#undef con_t
#undef val_t


