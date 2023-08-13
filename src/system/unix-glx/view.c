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
    mat4 temp;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    if(model->view) {
        glLoadMatrixf((GLfloat *) temp);
    } else {
        glLoadIdentity();
    }
    glMatrixMode(GL_MODELVIEW);
    for(int pid = 0; pid < model->props.size; pid++) {
        prop = (breadbox_prop_t *) model->props.data[pid];
        material = prop->material;
        glLoadMatrixf((GLfloat *) temp);
        glBegin(GL_TRIANGLES);
        glColor3f(material->color.r, material->color.g, material->color.b);
        for(int fid = 0; fid < prop->geometry->faces.size; fid++) {
            face = (breadbox_face_t *) prop->geometry->faces.data[fid];
            glNormal3f(face->normal[0], face->normal[1], face->normal[2]);
            glVertex3f(*face->a[0], *face->a[1], *face->a[2]);
            glVertex3f(*face->b[0], *face->b[1], *face->b[2]);
            glVertex3f(*face->c[0], *face->c[1], *face->c[2]);
        }
        glEnd();
    }
    glXSwapBuffers(DISPLAY, WINDOW);
}