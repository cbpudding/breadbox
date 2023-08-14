#include "breadbox.h"

void breadbox_camera_update(breadbox_camera_t *camera) {
    mat4 temp;
    glm_perspective(camera->fov, camera->aspect, camera->near, camera->far, camera->matrix);
    glm_mat4_identity(temp);
    temp[3][0] = camera->position[0];
    temp[3][1] = camera->position[1];
    temp[3][2] = camera->position[2];
    // We invert the translation matrix so the axes aren't reversed. In other
    // words, we want to move the world around the camera, not the camera around
    // the world. ~Alex
    glm_mat4_inv(temp, temp);
    glm_mat4_mul(camera->matrix, temp, camera->matrix);
    // ...
}

void breadbox_color_convert(vec4 victim, uint32_t color) {
    // Well I tried to line it up to be nice and readable... ~Alex
    victim[0] = (float)((color & 0xFF000000) >> 24) / 255.0;
    victim[1] = (float)((color & 0x00FF0000) >> 16) / 255.0;
    victim[2] = (float)((color & 0x0000FF00) >> 8) / 255.0;
    victim[3] = (float)(color & 0x000000FF) / 255.0;
}
