#include "breadbox.h"

void breadbox_model_init(breadbox_model_t *model) {
    // I am NOT including the standard library just to have NULL. ~Alex
    model->game = (void *)0;
    model->tick = 0;
}