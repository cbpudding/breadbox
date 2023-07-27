#include "breadbox.h"

void breadbox_matrix_identity(breadbox_matrix_t matrix) {
    for(int i = 0; i < 16; i++) {
        matrix[i] = (float)(i % 5 == 0);
    }
}