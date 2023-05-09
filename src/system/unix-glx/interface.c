#include <stdio.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include "breadbox.h"

const GLint attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

// Because who doesn't like global variables? *cries in ANSI C* ~Alex
Display *DISPLAY;
Window WINDOW;

int create_window(XVisualInfo *visinfo) {
    Colormap colormap;
    Window root;
    XSetWindowAttributes setwinattr;
    // TODO: If any of the following fails, how should it be handled? ~Alex
    root = DefaultRootWindow(DISPLAY);
    colormap = XCreateColormap(DISPLAY, root, visinfo->visual, AllocNone);
    setwinattr.colormap = colormap;
    setwinattr.event_mask = ExposureMask | KeyPressMask;
    WINDOW = XCreateWindow(DISPLAY, root, 0, 0, 640, 480, 0, visinfo->depth, InputOutput, visinfo->visual, CWColormap | CWEventMask, &setwinattr);
    return 0;
}

int main(int argc, char *argv[]) {
    int alive = 1;
    GLXContext context;
    breadbox_t engine;
    clock_t epoch = clock();
    XEvent event;
    XVisualInfo *visinfo;
    DISPLAY = XOpenDisplay(NULL);
    if(!DISPLAY) {
        puts("main: Unable to connect to X server!");
        return 1;
    }
    visinfo = glXChooseVisual(DISPLAY, 0, (int *)attr);
    if(!visinfo) {
        puts("main: No compatible visual found!");
        XCloseDisplay(DISPLAY);
        return 1;
    }
    if(create_window(visinfo)) {
        puts("main: Unable to create WINDOW!");
        XCloseDisplay(DISPLAY);
        return 1;
    }
    XMapWindow(DISPLAY, WINDOW);
    XStoreName(DISPLAY, WINDOW, "Breadbox");
    context = glXCreateContext(DISPLAY, visinfo, NULL, GL_TRUE);
    if(!context) {
        puts("main: Failed to create GL context!");
        XDestroyWindow(DISPLAY, WINDOW);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    // XSelectInput needs to happen after glXCreateContext because OpenGL fails
    // to load otherwise. ~Alex
    XSelectInput(DISPLAY, WINDOW, StructureNotifyMask);
    glXMakeCurrent(DISPLAY, WINDOW, context);
    glEnable(GL_DEPTH_TEST);
    while(alive) {
        if(XCheckWindowEvent(DISPLAY, WINDOW, StructureNotifyMask, &event)) {
            switch(event.type) {
                case DestroyNotify:
                    alive = 0;
                    break;
                case Expose:
                    // TODO: Fire BBMSG_VIEW ~Alex
                    break;
                default:
                    break;
            }
        }
    }
    glXMakeCurrent(DISPLAY, None, NULL);
    glXDestroyContext(DISPLAY, context);
    XCloseDisplay(DISPLAY);
    return 0;
}