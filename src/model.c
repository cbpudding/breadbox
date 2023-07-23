#include "breadbox.h"

void breadbox_model_free(breadbox_model_t *model) {
    breadbox_list_free(&model->geometry);
}

void breadbox_model_init(breadbox_model_t *model) {
    breadbox_list_init(&model->geometry);
    model->tick = 0;
}