#include "breadbox.h"

void breadbox_matrix_identity(breadbox_matrix_t matrix) {
    for(int i = 0; i < 16; i++) {
        matrix[i] = (float)(i % 5 == 0);
    }
}

void breadbox_matrix_multiply(breadbox_matrix_t result, breadbox_matrix_t a, breadbox_matrix_t b) {
    // We're storing a copy of the matrix before it overwrites result because
    // the result matrix could be used in the calculation. ~Alex
    breadbox_matrix_t victim;
    for(int i = 0; i < 16; i++) {
        victim[i] = 0.0;
        for(int j = 0; j < 4; j++) {
            victim[i] += a[(i & 12) | j] * b[(i & 3) | (j << 2)];
        }
    }
    for(int i = 0; i < 16; i++) {
        result[i] = victim[i];
    }
}