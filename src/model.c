#include <stddef.h>

#include "breadbox.h"

void breadbox_geometry_face(breadbox_face_t *face, vec3 *a, vec3 *b, vec3 *c) {
    vec3 u;
    vec3 v;
    face->a = a;
    face->b = b;
    face->c = c;
    glm_vec3_sub((float *)b, (float *)a, (float *)u);
    glm_vec3_sub((float *)c, (float *)a, (float *)v);
    // This is probably right... Let me know if it isn't. ~Alex
    face->normal[0] = (u[1] * v[2]) - (u[2] * v[1]);
    face->normal[1] = (u[2] * v[0]) - (u[0] * v[2]);
    face->normal[2] = (u[0] * v[1]) - (u[1] * v[0]);
}

void breadbox_geometry_free(breadbox_geometry_t *geometry) {
    breadbox_list_free(&geometry->faces);
    breadbox_list_free(&geometry->vertices);
}

void breadbox_geometry_init(breadbox_geometry_t *geometry) {
    breadbox_list_init(&geometry->faces);
    breadbox_list_init(&geometry->vertices);
}

void breadbox_model_free(breadbox_model_t *model) {
    breadbox_list_free(&model->props);
}

void breadbox_model_init(breadbox_model_t *model) {
    breadbox_list_init(&model->props);
    model->view = NULL;
}