#include "breadbox.h"

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

extern Display *DISPLAY;
extern Window WINDOW;

void breadbox_view(breadbox_model_t *model) {
    float angle;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Test triangle to be replaced by actual rendering code later. ~Alex
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // bananas ROTAT E ~Alex
    angle = (float)(model->tick % (BREADBOX_TICKRATE * 4)) / ((float)BREADBOX_TICKRATE * 4);
    glRotatef(angle * 360.0, 0.0, 1.0, 0.0);
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