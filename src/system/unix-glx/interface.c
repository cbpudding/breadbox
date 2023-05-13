#include <stdio.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include "breadbox.h"

#define NSEC_PER_TICK (1000000000 / BREADBOX_TICKRATE)

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
    struct timespec epoch;
    XEvent event;
    breadbox_message_t msg;
    struct timespec now;
    XVisualInfo *visinfo;
    XWindowAttributes winattr;
    if(clock_gettime(CLOCK_MONOTONIC, &epoch)) {
        puts("main: Failed to initialize timer");
        return 1;
    }
    // IMPORTANT NOTE: We should have a *proper* initialization function for the
    // engine, however this will do for development/testing. If this makes it
    // into the final release, you have my permission to slap me. ~Alex
    engine.model.tick = 0;
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
        if(clock_gettime(CLOCK_MONOTONIC, &now)) {
            puts("main: Failed to read the monotonic clock! Things might get weird!");
        // Breadbox treats BREADBOX_TICKRATE as the number of expected clock
        // ticks in a second. However, Unix likes to treat the time as a number
        // of seconds since a certain point. Nanoseconds are this awkward
        // secondary value we need to pay attention to. The easiest way to
        // measure time accurately would probably be to convert the time given
        // by clock_gettime into the number of expected ticks and comparing what
        // we believe the current tick is to that. Assuming the machine isn't
        // running too far behind, this also allows it to catch up in case one
        // tick was particularly taxing on the hardware. ~Alex
        } else if(((now.tv_sec * BREADBOX_TICKRATE) + (now.tv_nsec / NSEC_PER_TICK)) > engine.model.tick) {
            if(!++engine.model.tick) {
                // If anybody is crazy enough to run the engine for this long,
                // let me know about it. They're my kind of crazy. ~Alex
                puts(
                    "main: Did you seriously run this game for almost 7 years"
                    "straight? You have my respect. This message will now"
                    "self-destruct. ~Alex"
                );
                alive = 0;
                break;
            } else {
                msg = BBMSG_TICK;
                breadbox_publish(&engine, &msg);
            }
        }
        // For those of you wondering, there *is* a reason why I separated the
        // tick trigger from the other events. I consider BBMSG_TICK to be a
        // high priority message compared to the others because of how time
        // sensitive it is. I don't want it to get drowned out by events from X
        // or anything else. Therefore, to prevent potential issues from
        // occuring, I gave it its own dedicated if block above this one. ~Alex
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