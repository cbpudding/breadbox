#include "breadbox.h"

vec3 TEST_VERTICES[8] = {
    {-0.5, -0.5, -0.5},
    {0.5, -0.5, -0.5},
    {-0.5, 0.5, -0.5},
    {0.5, 0.5, -0.5},
    {-0.5, -0.5, 0.5},
    {0.5, -0.5, 0.5},
    {-0.5, 0.5, 0.5},
    {0.5, 0.5, 0.5}
};

vec3 CAMERA;
breadbox_face_t TEST_FACES[12];
breadbox_geometry_t TEST_GEOMETRY;
breadbox_material_t TEST_MATERIAL;
breadbox_prop_t TEST;
mat4 VIEW;

void breadbox_cleanup(breadbox_t *engine) {
    breadbox_geometry_free(&TEST_GEOMETRY);
}

void breadbox_init(breadbox_t *engine) {
    float aspect = (float)engine->subscriptions.width / (float)engine->subscriptions.height;
    breadbox_subscribe(&engine->subscriptions, BBMSG_AXIS0);
    breadbox_subscribe(&engine->subscriptions, BBMSG_AXIS1);
    breadbox_subscribe(&engine->subscriptions, BBMSG_AXIS2);
    glm_perspective(90.0, aspect, 0.125, 64.0, VIEW);
    engine->model.view = &VIEW;
    breadbox_subscribe(&engine->subscriptions, BBMSG_TICK);
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
    TEST_MATERIAL.color.r = 0.7;
    TEST_MATERIAL.color.g = 0.86;
    TEST_MATERIAL.color.b = 0.96;
    TEST.geometry = &TEST_GEOMETRY;
    TEST.material = &TEST_MATERIAL;
    glm_mat4_identity(TEST.matrix);
    breadbox_list_append(&engine->model.props, (void *)&TEST);
    // ...
    breadbox_info("Sandbox started");
}

void breadbox_update(breadbox_t *engine, breadbox_message_t msg) {
    float aspect = (float)engine->subscriptions.width / (float)engine->subscriptions.height;
    mat4 transform;
    switch(msg) {
        case BBMSG_AXIS0:
            breadbox_debug("BBMSG_AXIS0: %f", engine->subscriptions.axes[0]);
            break;
        case BBMSG_AXIS1:
            breadbox_debug("BBMSG_AXIS1: %f", engine->subscriptions.axes[1]);
            break;
        case BBMSG_AXIS2:
            breadbox_debug("BBMSG_AXIS2: %f", engine->subscriptions.axes[2]);
            break;
        case BBMSG_TICK:
            CAMERA[0] += engine->subscriptions.axes[0] * 0.05;
            CAMERA[1] += engine->subscriptions.axes[1] * 0.05;
            CAMERA[2] += engine->subscriptions.axes[2] * 0.05;
            glm_mat4_identity(transform);
            transform[3][0] = CAMERA[0];
            transform[3][1] = CAMERA[1];
            transform[3][2] = CAMERA[2];
            glm_perspective(90.0, aspect, 0.125, 64.0, VIEW);
            glm_mat4_mul(transform, VIEW, VIEW);
            break;
        default:
            break;
    }
}