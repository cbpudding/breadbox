#include <math.h>

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

void breadbox_matrix_order(breadbox_matrix_t result, breadbox_matrix_t victim) {
    // Again, this matrix is here just in case the input and output are the same
    // matrix. ~Alex
    breadbox_matrix_t temp;
    for(int i = 0; i < 16; i++) {
        temp[i] = victim[((i & 12) >> 2) | ((i & 3) << 2)];
    }
    for(int i = 0; i < 16; i++) {
        result[i] = temp[i];
    }
}

void breadbox_matrix_perspective(breadbox_matrix_t matrix, float aspect, float fov, float near, float far) {
    float f = 1.0 / tanf(fov / 2.0);
    matrix[0] = f / aspect;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
    matrix[4] = 0.0;
    matrix[5] = f;
    matrix[6] = 0.0;
    matrix[7] = 0.0;
    matrix[8] = 0.0;
    matrix[9] = 0.0;
    matrix[10] = (far + near) / (near - far);
    matrix[11] = (2.0 * far * near) / (near - far);
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = 1.0;
    matrix[15] = 0.0;
}