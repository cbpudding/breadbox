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