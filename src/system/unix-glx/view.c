#include <math.h>
#include <stdio.h>

#include "breadbox.h"

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

extern Display *DISPLAY;
extern int MAX_LIGHTS;
extern Window WINDOW;

extern float get_subtick(void);

void view(breadbox_model_t *model) {
    breadbox_face_t *face;
    breadbox_light_t *light;
    breadbox_prop_t *prop;
    int temp;
    static int total_lights = 0;
    // Reset the buffers and reconfigure the viewport ~Alex
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    if(model->view) {
        glLoadMatrixf((GLfloat *) model->view);
    } else {
        glLoadIdentity();
    }
    // glDisable and glEnable are expensive so it's best to only run them if we
    // need them. We'll disable lights if we have too many enabled and enable
    // lights if we have too many disabled. Of course, we'll also need to cap
    // the number at whatever OpenGL declares the max to be. ~Alex
    temp = model->lights.size >= MAX_LIGHTS ? MAX_LIGHTS : model->lights.size;
    if(temp != total_lights && temp >= 0) {
        if(temp > total_lights) {
            for(int i = total_lights; i < temp; i++) {
                glEnable(GL_LIGHT0 + i);
            }
        } else {
            for(int i = temp; i < total_lights; i++) {
                glDisable(GL_LIGHT0 + i);
            }
        }
        total_lights = temp;
    }
    // Finally, populate the information about the lights ~Alex
    for(int lid = 0; lid < model->lights.size; lid++) {
        light = (breadbox_light_t *)model->lights.data[lid];
        glLightfv(GL_LIGHT0 + lid, GL_AMBIENT, model->ambient);
        glLightfv(GL_LIGHT0 + lid, GL_DIFFUSE, light->diffuse);
        glLightfv(GL_LIGHT0 + lid, GL_POSITION, light->position);
        glLightfv(GL_LIGHT0 + lid, GL_SPECULAR, light->specular);
    }
    // Now we populate information about the props ~Alex
    glMatrixMode(GL_MODELVIEW);
    for(int pid = 0; pid < model->props.size; pid++) {
        prop = (breadbox_prop_t *) model->props.data[pid];
        glLoadMatrixf((GLfloat *) prop->matrix);
        glBegin(GL_TRIANGLES);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, prop->material->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, prop->material->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, prop->material->specular);
        for(int fid = 0; fid < prop->geometry->faces.size; fid++) {
            face = (breadbox_face_t *) prop->geometry->faces.data[fid];
            glNormal3f((*face->na)[0], (*face->na)[1], (*face->na)[2]);
            glVertex3f((*face->a)[0], (*face->a)[1], (*face->a)[2]);
            glNormal3f((*face->nb)[0], (*face->nb)[1], (*face->nb)[2]);
            glVertex3f((*face->b)[0], (*face->b)[1], (*face->b)[2]);
            glNormal3f((*face->nc)[0], (*face->nc)[1], (*face->nc)[2]);
            glVertex3f((*face->c)[0], (*face->c)[1], (*face->c)[2]);
        }
        glEnd();
    }
    glXSwapBuffers(DISPLAY, WINDOW);
}
