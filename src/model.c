#include <stddef.h>

#include "breadbox.h"

void breadbox_geometry_face(breadbox_face_t *face, breadbox_vertex_t *a, breadbox_vertex_t *b, breadbox_vertex_t *c) {
    face->a = a;
    face->b = b;
    face->c = c;
}

void breadbox_geometry_free(breadbox_geometry_t *geometry) {
    breadbox_list_free(&geometry->faces);
    breadbox_list_free(&geometry->vertices);
}

void breadbox_geometry_init(breadbox_geometry_t *geometry) {
    breadbox_list_init(&geometry->faces);
    breadbox_list_init(&geometry->vertices);
}

void breadbox_geometry_vertex(breadbox_vertex_t *vertex, float x, float y, float z) {
    vertex->x = x;
    vertex->y = y;
    vertex->z = z;
}

void breadbox_model_free(breadbox_model_t *model) {
    breadbox_list_free(&model->geometry);
}

void breadbox_model_init(breadbox_model_t *model) {
    breadbox_list_init(&model->geometry);
    model->tick = 0;
    model->view = NULL;
}