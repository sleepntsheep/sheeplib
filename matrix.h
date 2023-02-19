/* matrix.h - Simple matrix library
 *
 * Caution: this assume you give valid input, and does very little error handling.
 */

#ifndef matrix_num_t
#error "Must define matrix_num_t before including matrix.h."
#endif

#ifndef SHEEP_MATRIX_H
#define SHEEP_MATRIX_H

struct Matrix {
    int nrow;
    int ncol;
    matrix_num_t a[1];
};

struct Matrix *matrix_new(int nrow, int ncol);
struct Matrix *matrix_from_1darray(int nrow, int ncol, matrix_num_t *array);
void matrix_free(struct Matrix *m);

struct Matrix *matrix_identity(int n);
struct Matrix *matrix_zero(int n);

void matrix_set(struct Matrix *m, int row, int col, matrix_num_t x);
matrix_num_t matrix_get(struct Matrix *m, int row, int col);

matrix_num_t matrix_minor(struct Matrix *m, int row, int col);
matrix_num_t matrix_determinant(struct Matrix *m);
struct Matrix *matrix_transpose(struct Matrix *m);

struct Matrix *matrix_add(struct Matrix *a, struct Matrix *b);
struct Matrix *matrix_sub(struct Matrix *a, struct Matrix *b);
struct Matrix *matrix_mul(struct Matrix *a, struct Matrix *b);

struct Matrix *matrix_add_k(struct Matrix *m, matrix_num_t k);
struct Matrix *matrix_mul_k(struct Matrix *m, matrix_num_t k);

/* todo */
struct Matrix *matrix_inverse(struct Matrix *m);
struct Matrix *matrix_adjoint(struct Matrix *m);
struct Matrix *matrix_cofactor(struct Matrix *m);

#endif

#ifdef SHEEP_MATRIX_IMPLEMENTATION
#include <stdlib.h>
#include <stddef.h>

struct Matrix *matrix_new(int nrow, int ncol) {
    struct Matrix *m = NULL;
    size_t i, j;
    if (nrow < 1 && ncol < 1) return NULL;
    m = calloc(1, offsetof(struct Matrix, a) + (sizeof(matrix_num_t) * nrow * ncol));
    m->nrow = nrow;
    m->ncol = ncol;
    for (i = 1; i <= nrow; i++)
        for (j = 1; j <= ncol; j++)
            matrix_set(m, i, j, 0.0f);
    return m;
}

struct Matrix *matrix_identity(int n) {
    size_t i;
    struct Matrix *m = matrix_zero(n);
    for (i = 1; i <= n; i++)
        matrix_set(m, i, i, 1);
    return m;
}

struct Matrix *matrix_zero(int n) {
    size_t i, j;
    struct Matrix *m = matrix_new(n, n);
    for (i = 1; i <= n; i++)
        for (j = 1; j <= n; j++)
            matrix_set(m, i, j, 0);
    return m;
}

void matrix_set(struct Matrix *m, int row, int col, matrix_num_t x) {
    if (row > m->nrow || col > m->ncol) return;
    m->a[(row - 1) * m->ncol + (col - 1)] = x;
}

matrix_num_t matrix_get(struct Matrix *m, int row, int col) {
    if (row > m->nrow || col > m->ncol) return 0;
    return m->a[(row - 1) * m->ncol + (col - 1)];
}

struct Matrix *matrix_transpose(struct Matrix *m) {
    struct Matrix *r = matrix_new(m->ncol, m->nrow);
    size_t i, j;
    for (i = 1; i <= m->nrow; i++)
        for (j = 1; j <= m->ncol; j++)
            matrix_set(r, j, i, matrix_get(m, i, j));
    return r;
}

struct Matrix *matrix_add(struct Matrix *a, struct Matrix *b) {
    struct Matrix *r = NULL;
    size_t i, j;
    if (a->ncol != b->ncol || a->nrow != b->nrow) return NULL;
    r = matrix_new(a->ncol, a->nrow);
    for (i = 1; i <= a->nrow; i++)
        for (j = 1; j <= a->ncol; j++)
            matrix_set(r, i, j, matrix_get(a, i, j) + matrix_get(b, i, j));
    return r;
}

struct Matrix *matrix_sub(struct Matrix *a, struct Matrix *b) {
    struct Matrix *r = NULL;
    size_t i, j;
    if (a->ncol != b->ncol || a->nrow != b->nrow) return NULL;
    r = matrix_new(a->nrow, a->ncol);
    for (i = 1; i <= a->nrow; i++)
        for (j = 1; j <= a->ncol; j++)
            matrix_set(r, i, j, matrix_get(a, i, j) - matrix_get(b, i, j));
    return r;
}

struct Matrix *matrix_mul(struct Matrix *a, struct Matrix *b) {
    struct Matrix *r = NULL;
    size_t i, j, k;
    if (a->ncol != b->nrow) return NULL;
    r = matrix_new(a->nrow, b->ncol);
    for (i = 1; i <= r->nrow; i++) {
        for (j = 1; j <= r->ncol; j++) {
            matrix_num_t x = 0;
            for (k = 0; k < a->ncol; k++) {
                x += matrix_get(a, i, k) * matrix_get(b, k, j);
            }
            matrix_set(r, i, j, x);
        }
    }
    return r;
}

matrix_num_t matrix_minor(struct Matrix *m, int row, int col) {
    struct Matrix *r = matrix_new(m->nrow - 1, m->ncol - 1);
    size_t i, j, rj, ri;
    matrix_num_t det;
    for (i = 1, ri = 1; i <= m->nrow; i++) {
        if (i == row) continue;
        for (j = 1, rj = 1; j <= m->ncol; j++) {
            if (j == col) continue;
            matrix_set(r, ri, rj, matrix_get(m, i, j));
            rj++;
        }
        ri++;
    }
    det = matrix_determinant(r);
    matrix_free(r);
    return det;
}

matrix_num_t matrix_determinant(struct Matrix *m) {
    size_t j;
    matrix_num_t det = 0;
    if (m->nrow != m->ncol) return 0;
    if (m->nrow == 1 && m->ncol == 1) return matrix_get(m, 1, 1);
    for (j = 0; j < m->ncol; j++) {
        det += ((j & 1) ? 1 : -1) * matrix_get(m, 1, j) * matrix_minor(m, 1, j);
    }
    return det;
}

struct Matrix *matrix_add_k(struct Matrix *m, matrix_num_t k) {
    size_t i, j;
    struct Matrix *r = matrix_new(m->nrow, m->ncol);
    for (i = 1; i <= m->nrow; i++)
        for (j = 1; j <= m->ncol; j++)
            matrix_set(r, i, j, matrix_get(m, i, j) + k);
    return r;
}

struct Matrix *matrix_mul_k(struct Matrix *m, matrix_num_t k) {
    struct Matrix *r = matrix_new(m->nrow, m->ncol);
    size_t i, j;
    for (i = 1; i <= m->nrow; i++)
        for (j = 1; j <= m->ncol; j++)
            matrix_set(r, i, j, matrix_get(m, i, j) * k);
    return r;
}

struct Matrix *matrix_from_1darray(int nrow, int ncol, matrix_num_t *array) {
    struct Matrix *r = matrix_new(nrow, ncol);
    size_t i, j;
    for (i = 1; i <= nrow; i++)
        for (j = 1; j <= ncol; j++)
            matrix_set(r, i, j, array[(i-1) * ncol + (j-1)]);
    return r;
}

void matrix_free(struct Matrix *m) {
    free(m);
}

struct Matrix *matrix_cofactor(struct Matrix *m) {
    struct Matrix *r = matrix_new(m->nrow, m->ncol);
    size_t i, j;
    for (i = 1; i <= m->nrow; i++)
        for (j = 1; j <= m->ncol; j++)
            matrix_set(r, i, j, (((i + j) & 1) ? -1 : 1) * matrix_minor(m, i, j));
    return r;
}

struct Matrix *matrix_adjoint(struct Matrix *m) {
    return matrix_transpose(matrix_cofactor(m));
}

struct Matrix *matrix_inverse(struct Matrix *m) {
    struct Matrix *adjoint = matrix_adjoint(m);
    return matrix_mul_k(adjoint, 1.0f / matrix_determinant(m));
}

#endif /* SHEEP_MATRIX_IMPLEMENTATION */

