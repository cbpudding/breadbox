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

breadbox_camera_t CAMERA;
breadbox_face_t TEST_FACES[12];
breadbox_geometry_t TEST_GEOMETRY;
breadbox_material_t TEST_MATERIAL;
breadbox_prop_t TEST;

void breadbox_cleanup(breadbox_t *engine) {
    breadbox_geometry_free(&TEST_GEOMETRY);
}

void breadbox_init(breadbox_t *engine) {
    CAMERA.aspect = (float)engine->subscriptions.width / (float)engine->subscriptions.height;
    CAMERA.far = 64.0;
    CAMERA.fov = 90.0;
    CAMERA.near = 0.125;
    glm_vec3_zero(CAMERA.position);
    breadbox_camera_update(&CAMERA);
    engine->model.view = &CAMERA.matrix;
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
    TEST_MATERIAL.color[0] = 0.7;
    TEST_MATERIAL.color[1] = 0.86;
    TEST_MATERIAL.color[2] = 0.96;
    TEST.geometry = &TEST_GEOMETRY;
    TEST.material = &TEST_MATERIAL;
    glm_mat4_identity(TEST.matrix);
    breadbox_list_append(&engine->model.props, (void *)&TEST);
    // ...
    breadbox_info("Sandbox started");
}

void breadbox_update(breadbox_t *engine, breadbox_message_t msg) {
    switch(msg) {
        case BBMSG_TICK:
            CAMERA.aspect = (float)engine->subscriptions.width / (float)engine->subscriptions.height;
            CAMERA.position[0] += engine->subscriptions.axes[0] * 0.05;
            CAMERA.position[1] += engine->subscriptions.axes[1] * 0.05;
            CAMERA.position[2] += engine->subscriptions.axes[2] * 0.05;
            breadbox_camera_update(&CAMERA);
            break;
        default:
            break;
    }
}