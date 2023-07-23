#include "breadbox.h"

breadbox_vertex_t TEST_VERTICES[3];
breadbox_face_t TEST_FACE;
breadbox_geometry_t TEST;

void breadbox_cleanup(breadbox_t *engine) {
    breadbox_list_free(&TEST.faces);
    breadbox_list_free(&TEST.vertices);
}

void breadbox_init(breadbox_t *engine) {
    breadbox_subscribe(&engine->subscriptions, BBMSG_TICK);
    TEST_VERTICES[0].x = -0.5;
    TEST_VERTICES[0].y = -0.5;
    TEST_VERTICES[0].z = 0.0;
    TEST_VERTICES[1].x = 0.0;
    TEST_VERTICES[1].y = 0.5;
    TEST_VERTICES[1].z = 0.0;
    TEST_VERTICES[2].x = 0.5;
    TEST_VERTICES[2].y = -0.5;
    TEST_VERTICES[2].z = 0.0;
    TEST_FACE.a = &TEST_VERTICES[0];
    TEST_FACE.b = &TEST_VERTICES[1];
    TEST_FACE.c = &TEST_VERTICES[2];
    breadbox_list_init(&TEST.vertices);
    breadbox_list_append(&TEST.vertices, (void *)&TEST_VERTICES[0]);
    breadbox_list_append(&TEST.vertices, (void *)&TEST_VERTICES[1]);
    breadbox_list_append(&TEST.vertices, (void *)&TEST_VERTICES[2]);
    breadbox_list_init(&TEST.faces);
    breadbox_list_append(&TEST.faces, (void *)&TEST_FACE);
    breadbox_list_append(&engine->model.geometry, (void *)&TEST);
    // ...
    breadbox_info("Sandbox started");
}

void breadbox_update(breadbox_t *engine, breadbox_message_t *msg) {
    // ...
}