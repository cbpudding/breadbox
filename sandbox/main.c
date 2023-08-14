#include "breadbox.h"

vec3 TEST_NORMALS[5] = {
    {0.0, 1.0, 0.0},
    {1.0, -1.0, 1.0},
    {-1.0, -1.0, 1.0},
    {-1.0, -1.0, -1.0},
    {1.0, -1.0, -1.0}
};

vec3 TEST_VERTICES[5] = {
    {0.0, 0.5, 0.0},
    {0.5, -0.5, 0.5},
    {-0.5, -0.5, 0.5},
    {-0.5, -0.5, -0.5},
    {0.5, -0.5, -0.5}
};

breadbox_camera_t CAMERA;
breadbox_face_t TEST_FACES[6];
breadbox_geometry_t TEST_GEOMETRY;
breadbox_material_t TEST_MATERIAL;
breadbox_prop_t TEST;

void breadbox_cleanup(breadbox_t *engine) {
    breadbox_geometry_free(&TEST_GEOMETRY);
}

void breadbox_init(breadbox_t *engine) {
    // Subscriptions
    breadbox_subscribe(&engine->subscriptions, BBMSG_TICK);
    // Camera
    CAMERA.aspect = (float)engine->subscriptions.width / (float)engine->subscriptions.height;
    CAMERA.far = 64.0;
    CAMERA.fov = 90.0;
    CAMERA.near = 0.125;
    glm_vec3_zero(CAMERA.position);
    breadbox_camera_update(&CAMERA);
    engine->model.view = &CAMERA.matrix;
    // Prop Geometry
    for(int i = 0; i < 4; i++) {
        TEST_FACES[i].a = &TEST_VERTICES[0];
        TEST_FACES[i].na = &TEST_NORMALS[0];
        TEST_FACES[i].b = &TEST_VERTICES[i + 1];
        TEST_FACES[i].nb = &TEST_NORMALS[i + 1];
        TEST_FACES[i].c = &TEST_VERTICES[((i + 1) % 4) + 1];
        TEST_FACES[i].nc = &TEST_NORMALS[((i + 1) % 4) + 1];
    }
    TEST_FACES[4].a = &TEST_VERTICES[1];
    TEST_FACES[4].na = &TEST_NORMALS[1];
    TEST_FACES[4].b = &TEST_VERTICES[3];
    TEST_FACES[4].nb = &TEST_NORMALS[3];
    TEST_FACES[4].c = &TEST_VERTICES[2];
    TEST_FACES[4].nc = &TEST_NORMALS[2];
    TEST_FACES[5].a = &TEST_VERTICES[1];
    TEST_FACES[5].na = &TEST_NORMALS[1];
    TEST_FACES[5].b = &TEST_VERTICES[4];
    TEST_FACES[5].nb = &TEST_NORMALS[4];
    TEST_FACES[5].c = &TEST_VERTICES[3];
    TEST_FACES[5].nc = &TEST_NORMALS[3];
    breadbox_geometry_init(&TEST_GEOMETRY);
    for(int i = 0; i < 5; i++) {
        breadbox_list_append(&TEST_GEOMETRY.vertices, (void *)&TEST_VERTICES[i]);
    }
    for(int i = 0; i < 5; i++) {
        breadbox_list_append(&TEST_GEOMETRY.normals, (void *)&TEST_NORMALS[i]);
    }
    for(int i = 0; i < 6; i++) {
        breadbox_list_append(&TEST_GEOMETRY.faces, (void *)&TEST_FACES[i]);
    }
    // Prop Material
    breadbox_color_convert(TEST_MATERIAL.ambient, 0x000000ff);
    breadbox_color_convert(TEST_MATERIAL.diffuse, 0xffffffff);
    breadbox_color_convert(TEST_MATERIAL.specular, 0x545454ff);
    // Prop
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
            // This is here because we technically need to handle ALL cases,
            // even if we don't do anything with those cases. ~Alex
            break;
    }
}