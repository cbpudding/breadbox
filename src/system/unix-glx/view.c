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
    float angle;
    breadbox_face_t *face;
    breadbox_list_iter_t faces;
    breadbox_geometry_t *geometry;
    float r, g, b, t;
    breadbox_list_iter_t renderable;
    breadbox_list_node_t *victim;
    breadbox_list_iter(&model->geometry, &renderable);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Test projection matrix to be replaced later
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // bananas ROTAT E ~Alex
    angle = fmodf(get_subtick(), (float)BREADBOX_TICKRATE) / (float)BREADBOX_TICKRATE;
    glRotatef(angle * 360.0, 0.0, 1.0, 0.0);
    // End test projection
    // Code for initializing color cycling
    r = 0.0;
    g = 0.0;
    b = 1.0;
    while((victim = breadbox_list_next(&renderable))) {
        geometry = (breadbox_geometry_t *)victim->data;
        breadbox_list_iter(&geometry->faces, &faces);
        glBegin(GL_TRIANGLES);
        // Cycle colors every face for testing
        t = b;
        b = g;
        g = r;
        r = t;
        glColor3f(r, g, b);
        while((victim = breadbox_list_next(&faces))) {
            face = (breadbox_face_t *)victim->data;
            glVertex3f(face->a->x, face->a->y, face->a->z);
            glVertex3f(face->b->x, face->b->y, face->b->z);
            glVertex3f(face->c->x, face->c->y, face->c->z);
        }
        glEnd();
    }
    glXSwapBuffers(DISPLAY, WINDOW);
}