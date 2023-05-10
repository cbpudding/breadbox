#include <stdio.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include "breadbox.h"

#define CLOCKS_PER_TICK (CLOCKS_PER_SEC / BREADBOX_TICKRATE)

const GLint GL_ATTR[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

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
    breadbox_message_t msg;
    XVisualInfo *visinfo;
    XWindowAttributes winattr;
    // IMPORTANT NOTE: We should have a *proper* initialization function for the
    // engine, however this will do for development/testing. If this makes it
    // into the final release, you have my permission to slap me. ~Alex
    engine.tick = 0;
    DISPLAY = XOpenDisplay(NULL);
    if(!DISPLAY) {
        puts("main: Unable to connect to X server!");
        return 1;
    }
    visinfo = glXChooseVisual(DISPLAY, 0, (int *)GL_ATTR);
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
    // NOTE: If the window ever gets resized, then we'll need to run these two
    // again. ~Alex
    XGetWindowAttributes(DISPLAY, WINDOW, &winattr);
    glViewport(0, 0, winattr.width, winattr.height);
    while(alive) {
        // NOTE: If we ever decide to add a multiplayer, this will cause slight
        // timing differences between platforms. Is there a more portable or
        // accurate way to do this? ~Alex
        if((clock() - epoch) > (engine.tick * CLOCKS_PER_TICK)) {
            printf("%lu > %lu", clock() - epoch, engine.tick * CLOCKS_PER_TICK);
            engine.tick++;
            msg = BBMSG_TICK;
            breadbox_publish(&engine, &msg);
        }
        if(XCheckWindowEvent(DISPLAY, WINDOW, StructureNotifyMask, &event)) {
            switch(event.type) {
                case DestroyNotify:
                    alive = 0;
                    break;
                default:
                    break;
            }
        } else {
            msg = BBMSG_VIEW;
            breadbox_publish(&engine, &msg);
        }
    }
    glXMakeCurrent(DISPLAY, None, NULL);
    glXDestroyContext(DISPLAY, context);
    XCloseDisplay(DISPLAY);
    return 0;
}