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
    breadbox_list_iter_t renderable;
    breadbox_list_node_t *victim;
    breadbox_list_iter(&model->renderable, &renderable);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Test projection matrix to be replaced later
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // bananas ROTAT E ~Alex
    angle = fmodf(get_subtick(), (float)BREADBOX_TICKRATE) / (float)BREADBOX_TICKRATE;
    glRotatef(angle * 360.0, 0.0, 1.0, 0.0);
    // End test projection
    while((victim = breadbox_list_next(&renderable))) {
        // TODO: Render the objects in this list
    }
    // Test triangle to be replaced by actual rendering code later. ~Alex
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-0.5, -0.5);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0.0, 0.5);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0.5, -0.5);
    glEnd();
    // End of test triangle
    glXSwapBuffers(DISPLAY, WINDOW);
}