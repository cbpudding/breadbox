#include <stddef.h>

#include "breadbox.h"

void breadbox_geometry_face(breadbox_face_t *face, breadbox_vertex_t *a, breadbox_vertex_t *b, breadbox_vertex_t *c) {
    breadbox_vertex_t u;
    breadbox_vertex_t v;
    face->a = a;
    face->b = b;
    face->c = c;
    u.x = b->x - a->x;
    u.y = b->y - a->y;
    u.z = b->z - a->z;
    v.x = c->x - a->x;
    v.y = c->y - a->y;
    v.z = c->z - a->z;
    face->normal.x = (u.y * v.z) - (u.z * v.y);
    face->normal.y = (u.z * v.x) - (u.x * v.z);
    face->normal.z = (u.x * v.y) - (u.y * v.x);
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
    breadbox_list_free(&model->props);
}

void breadbox_model_init(breadbox_model_t *model) {
    breadbox_list_init(&model->props);
    model->tick = 0;
    model->view = NULL;
}