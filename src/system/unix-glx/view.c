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
    breadbox_material_t *material;
    breadbox_prop_t *prop;
    breadbox_matrix_t temp;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    if(model->view) {
        breadbox_matrix_order(temp, (float *) model->view);
        glLoadMatrixf((GLfloat *) temp);
    } else {
        glLoadIdentity();
    }
    glMatrixMode(GL_MODELVIEW);
    for(int pid = 0; pid < model->props.size; pid++) {
        prop = (breadbox_prop_t *) model->props.data[pid];
        material = prop->material;
        breadbox_matrix_order(temp, (float *) &prop->matrix);
        glLoadMatrixf((GLfloat *) temp);
        glBegin(GL_TRIANGLES);
        glColor3f(material->color.r, material->color.g, material->color.b);
        for(int fid = 0; fid < prop->geometry->faces.size; fid++) {
            face = (breadbox_face_t *) prop->geometry->faces.data[fid];
            glNormal3f(face->normal.x, face->normal.y, face->normal.z);
            glVertex3f(face->a->x, face->a->y, face->a->z);
            glVertex3f(face->b->x, face->b->y, face->b->z);
            glVertex3f(face->c->x, face->c->y, face->c->z);
        }
        glEnd();
    }
    glXSwapBuffers(DISPLAY, WINDOW);
}