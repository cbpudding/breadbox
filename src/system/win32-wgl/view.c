#include "breadbox.h"

#include <GL/gl.h>
#include <windows.h>


extern HDC devicecontext;
extern HWND window;

extern float get_subtick();

void view(breadbox_model_t *model) {
    float angle;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Test triangle to be replaced by actual rendering code later. ~Alex
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // bananas ROTAT E ~Alex
    angle = fmodf(get_subtick(), (float)BREADBOX_TICKRATE) / (float)BREADBOX_TICKRATE;
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
    SwapBuffers(devicecontext);
}