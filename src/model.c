#include <stddef.h>

#include "breadbox.h"

void breadbox_geometry_free(breadbox_geometry_t *geometry) {
    breadbox_list_free(&geometry->faces);
    breadbox_list_free(&geometry->normals);
    breadbox_list_free(&geometry->vertices);
}

void breadbox_geometry_init(breadbox_geometry_t *geometry) {
    breadbox_list_init(&geometry->faces);
    breadbox_list_init(&geometry->normals);
    breadbox_list_init(&geometry->vertices);
}

void breadbox_model_free(breadbox_model_t *model) {
    breadbox_list_free(&model->props);
}

void breadbox_model_init(breadbox_model_t *model) {
    breadbox_list_init(&model->props);
    model->view = NULL;
}
