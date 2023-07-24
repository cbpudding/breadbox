#include "breadbox.h"

breadbox_vertex_t TEST_VERTICES[8];
breadbox_face_t TEST_FACES[12];
breadbox_geometry_t TEST;

void breadbox_cleanup(breadbox_t *engine) {
    breadbox_list_free(&TEST.faces);
    breadbox_list_free(&TEST.vertices);
}

void breadbox_init(breadbox_t *engine) {
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
    breadbox_list_init(&TEST.vertices);
    for(int i = 0; i < 8; i++) {
        breadbox_list_append(&TEST.vertices, (void *)&TEST_VERTICES[i]);
    }
    breadbox_list_init(&TEST.faces);
    for(int i = 0; i < 12; i++) {
        breadbox_list_append(&TEST.faces, (void *)&TEST_FACES[i]);
    }
    breadbox_list_append(&engine->model.geometry, (void *)&TEST);
    // ...
    breadbox_info("Sandbox started");
}

void breadbox_update(breadbox_t *engine, breadbox_message_t *msg) {
    // ...
}