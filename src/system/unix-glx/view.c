#include <math.h>
#include <stdio.h>

#include "breadbox.h"

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

extern Display *DISPLAY;
extern Window WINDOW;

extern float get_subtick();

void view(breadbox_model_t *model) {
    breadbox_face_t *face;
    breadbox_list_iter_t faces;
    breadbox_material_t *material;
    breadbox_prop_t *prop;
    breadbox_list_iter_t props;
    breadbox_matrix_t temp;
    breadbox_list_node_t *victim;
    breadbox_list_iter(&model->props, &props);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    if(model->view) {
        breadbox_matrix_order(temp, (float *) model->view);
        glLoadMatrixf((GLfloat *) temp);
    } else {
        glLoadIdentity();
    }
    glMatrixMode(GL_MODELVIEW);
    while((victim = breadbox_list_next(&props))) {
        prop = (breadbox_prop_t *)victim->data;
        material = prop->material;
        breadbox_matrix_order(temp, (float *) &prop->matrix);
        glLoadMatrixf((GLfloat *) temp);
        glBegin(GL_TRIANGLES);
        glColor3f(material->color.r, material->color.g, material->color.b);
        breadbox_list_iter(&prop->geometry->faces, &faces);
        while((victim = breadbox_list_next(&faces))) {
            face = (breadbox_face_t *)victim->data;
            glNormal3f(face->normal.x, face->normal.y, face->normal.z);
            glVertex3f(face->a->x, face->a->y, face->a->z);
            glVertex3f(face->b->x, face->b->y, face->b->z);
            glVertex3f(face->c->x, face->c->y, face->c->z);
        }
        glEnd();
    }
    glXSwapBuffers(DISPLAY, WINDOW);
}