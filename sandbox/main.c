#include "breadbox.h"

breadbox_vertex_t TEST_VERTICES[8];
breadbox_face_t TEST_FACES[12];
breadbox_geometry_t TEST_GEOMETRY;
breadbox_prop_t TEST;
breadbox_matrix_t VIEW;

void breadbox_cleanup(breadbox_t *engine) {
    breadbox_geometry_free(&TEST_GEOMETRY);
}

void breadbox_init(breadbox_t *engine) {
    breadbox_matrix_identity((float *) &VIEW);
    engine->model.view = &VIEW;
    breadbox_subscribe(&engine->subscriptions, BBMSG_TICK);
    breadbox_geometry_vertex(&TEST_VERTICES[0], -0.5, -0.5, -0.5);
    breadbox_geometry_vertex(&TEST_VERTICES[1], 0.5, -0.5, -0.5);
    breadbox_geometry_vertex(&TEST_VERTICES[2], -0.5, 0.5, -0.5);
    breadbox_geometry_vertex(&TEST_VERTICES[3], 0.5, 0.5, -0.5);
    breadbox_geometry_vertex(&TEST_VERTICES[4], -0.5, -0.5, 0.5);
    breadbox_geometry_vertex(&TEST_VERTICES[5], 0.5, -0.5, 0.5);
    breadbox_geometry_vertex(&TEST_VERTICES[6], -0.5, 0.5, 0.5);
    breadbox_geometry_vertex(&TEST_VERTICES[7], 0.5, 0.5, 0.5);
    breadbox_geometry_face(&TEST_FACES[0], &TEST_VERTICES[4], &TEST_VERTICES[6], &TEST_VERTICES[7]);
    breadbox_geometry_face(&TEST_FACES[1], &TEST_VERTICES[4], &TEST_VERTICES[7], &TEST_VERTICES[5]);
    breadbox_geometry_face(&TEST_FACES[2], &TEST_VERTICES[5], &TEST_VERTICES[7], &TEST_VERTICES[3]);
    breadbox_geometry_face(&TEST_FACES[3], &TEST_VERTICES[5], &TEST_VERTICES[3], &TEST_VERTICES[1]);
    breadbox_geometry_face(&TEST_FACES[4], &TEST_VERTICES[1], &TEST_VERTICES[3], &TEST_VERTICES[2]);
    breadbox_geometry_face(&TEST_FACES[5], &TEST_VERTICES[1], &TEST_VERTICES[2], &TEST_VERTICES[0]);
    breadbox_geometry_face(&TEST_FACES[6], &TEST_VERTICES[0], &TEST_VERTICES[2], &TEST_VERTICES[6]);
    breadbox_geometry_face(&TEST_FACES[7], &TEST_VERTICES[0], &TEST_VERTICES[4], &TEST_VERTICES[6]);
    breadbox_geometry_face(&TEST_FACES[8], &TEST_VERTICES[6], &TEST_VERTICES[2], &TEST_VERTICES[3]);
    breadbox_geometry_face(&TEST_FACES[9], &TEST_VERTICES[6], &TEST_VERTICES[3], &TEST_VERTICES[7]);
    breadbox_geometry_face(&TEST_FACES[10], &TEST_VERTICES[0], &TEST_VERTICES[4], &TEST_VERTICES[5]);
    breadbox_geometry_face(&TEST_FACES[11], &TEST_VERTICES[0], &TEST_VERTICES[5], &TEST_VERTICES[1]);
    breadbox_geometry_init(&TEST_GEOMETRY);
    for(int i = 0; i < 8; i++) {
        breadbox_list_append(&TEST_GEOMETRY.vertices, (void *)&TEST_VERTICES[i]);
    }
    for(int i = 0; i < 12; i++) {
        breadbox_list_append(&TEST_GEOMETRY.faces, (void *)&TEST_FACES[i]);
    }
    TEST.geometry = &TEST_GEOMETRY;
    breadbox_list_append(&engine->model.props, (void *)&TEST);
    // ...
    breadbox_info("Sandbox started");
}

void breadbox_update(breadbox_t *engine, breadbox_message_t *msg) {
    // ...
}