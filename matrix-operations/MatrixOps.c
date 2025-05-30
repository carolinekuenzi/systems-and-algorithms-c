#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int nrows;
    int ncols;
    int **array;
} matrix;

bool dimSum(const matrix *m1, const matrix *m2) {
    return m1 && m2 && m1->nrows > 0 && m1->ncols > 0 &&
           m1->nrows == m2->nrows && m1->ncols == m2->ncols;
}

bool dimProd(const matrix *m1, const matrix *m2) {
    return m1 && m2 && m1->ncols == m2->nrows &&
           m1->nrows > 0 && m1->ncols > 0 && m2->ncols > 0;
}

matrix* matrix_sum(const matrix *m1, const matrix *m2) {
    if (!dimSum(m1, m2)) {
        fprintf(stderr, "Dimension mismatch for addition.\n");
        return NULL;
    }

    matrix *sum = malloc(sizeof(matrix));
    if (!sum) return NULL;

    sum->nrows = m1->nrows;
    sum->ncols = m1->ncols;
    sum->array = malloc(sum->nrows * sizeof(int*));
    if (!sum->array) { free(sum); return NULL; }

    for (int i = 0; i < sum->nrows; i++) {
        sum->array[i] = malloc(sum->ncols * sizeof(int));
        if (!sum->array[i]) {
            for (int k = 0; k < i; k++) free(sum->array[k]);
            free(sum);
            return NULL;
        }
        for (int j = 0; j < sum->ncols; j++) {
            sum->array[i][j] = m1->array[i][j] + m2->array[i][j];
        }
    }

    return sum;
}

matrix* matrix_prod(const matrix *m1, const matrix *m2) {
    if (!dimProd(m1, m2)) {
        fprintf(stderr, "Dimension mismatch for multiplication.\n");
        return NULL;
    }

    matrix *prod = malloc(sizeof(matrix));
    if (!prod) return NULL;

    prod->nrows = m1->nrows;
    prod->ncols = m2->ncols;
    prod->array = malloc(prod->nrows * sizeof(int*));
    if (!prod->array) { free(prod); return NULL; }

    for (int i = 0; i < prod->nrows; i++) {
        prod->array[i] = malloc(prod->ncols * sizeof(int));
        if (!prod->array[i]) {
            for (int k = 0; k < i; k++) free(prod->array[k]);
            free(prod);
            return NULL;
        }
        for (int j = 0; j < prod->ncols; j++) {
            prod->array[i][j] = 0;
            for (int k = 0; k < m1->ncols; k++) {
                prod->array[i][j] += m1->array[i][k] * m2->array[k][j];
            }
        }
    }

    return prod;
}
