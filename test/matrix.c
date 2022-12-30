#include <stdio.h>
#define SHEEP_MATRIX_IMPLEMENTATION
#define matrix_num_t double
#include "../matrix.h"

int main(void) {
    struct Matrix *i = matrix_identity(4);
    struct Matrix *i2 = matrix_from_1darray(4, 4, (double[]){
        1, 2, 3, 4,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
    });

    struct Matrix *r = matrix_add(i, i2);

    for (size_t i = 1; i <= r->nrow; i++) {
        for (size_t j = 1; j <= r->ncol; j++) {
            printf("%.1lf ", matrix_get(r, i, j));
        }
        printf("\n");
    }
    return 0;
}

