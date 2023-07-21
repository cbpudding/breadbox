#include "breadbox.h"

void breadbox_model_free(breadbox_model_t *model) {
    breadbox_list_free(&model->renderable);
}

void breadbox_model_init(breadbox_model_t *model) {
    breadbox_list_init(&model->renderable);
    model->tick = 0;
}