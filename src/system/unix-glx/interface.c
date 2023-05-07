#include <stdio.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include "system.h"

// I know that global variables are bad, but I really need a way to clean up
// the main function. ~Alex
GLXContext context;
Display *display;
Window window;

int main(int argc, char *argv[]) {
    int alive = 1;
    GLint attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    Colormap colormap;
    XEvent event;
    XSetWindowAttributes setwinattr;
    XVisualInfo *visinfo;
    XWindowAttributes winattr;
    if(!(display = XOpenDisplay(NULL))) {
        puts("Unable to connect to X server!");
        return 1;
    }
    if(!(visinfo = glXChooseVisual(display, 0, attr))) {
        puts("No compatible visual found");
        XCloseDisplay(display);
        return 1;
    }
    colormap = XCreateColormap(display, DefaultRootWindow(display), visinfo->visual, AllocNone);
    setwinattr.colormap = colormap;
    setwinattr.event_mask = ExposureMask | KeyPressMask;
    window = XCreateWindow(display, DefaultRootWindow(display), 0, 0, 640, 480, 0, visinfo->depth, InputOutput, visinfo->visual, CWColormap | CWEventMask, &setwinattr);
    XMapWindow(display, window);
    XStoreName(display, window, "Breadbox");
    context = glXCreateContext(display, visinfo, NULL, GL_TRUE);
    // XSelectInput needs to happen after glXCreateContext because OpenGL fails
    // to load otherwise. ~Alex
    XSelectInput(display, window, StructureNotifyMask);
    glXMakeCurrent(display, window, context);
    glEnable(GL_DEPTH_TEST);
    while(alive) {
        XNextEvent(display, &event);
        switch(event.type) {
            case DestroyNotify:
                alive = 0;
                break;
            case Expose:
                XGetWindowAttributes(display, window, &winattr);
                glViewport(0, 0, winattr.width, winattr.height);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glBegin(GL_TRIANGLES);
                glColor3f(1.0, 0.0, 0.0);
                glVertex2f(-0.5, -0.5);
                glColor3f(0.0, 1.0, 0.0);
                glVertex2f(0.0, 0.5);
                glColor3f(0.0, 0.0, 1.0);
                glVertex2f(0.5, -0.5);
                glEnd();
                glXSwapBuffers(display, window);
                break;
            default:
                break;
        }
    }
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XCloseDisplay(display);
    return 0;
}