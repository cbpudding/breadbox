#include <stdio.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include "breadbox.h"

#define EVENT_MASK ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask
#define NSEC_PER_TICK (1000000000 / BREADBOX_TICKRATE)

extern void view(breadbox_model_t *model);

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
    int expected_tick;
    breadbox_message_t msg;
    struct timespec now;
    XVisualInfo *visinfo;
    XWindowAttributes winattr;
    breadbox_model_init(&engine.model);
    breadbox_subscription_init(&engine.subscriptions);
    breadbox_init(&engine);
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
    XSelectInput(DISPLAY, WINDOW, EVENT_MASK);
    glXMakeCurrent(DISPLAY, WINDOW, context);
    glEnable(GL_DEPTH_TEST);
    // NOTE: If the window ever gets resized, then we'll need to run these two
    // again. ~Alex
    XGetWindowAttributes(DISPLAY, WINDOW, &winattr);
    glViewport(0, 0, winattr.width, winattr.height);
    // We don't want to deal with event spam from X so we'll tell it that we're
    // only looking for input changes and we don't want auto-repeating keys. ~Alex
    XAutoRepeatOff(DISPLAY);
    // We're putting epoch initialization here to make the difference between
    // the epoch and the first timestamp as little as possible so it doesn't
    // complaining about missing ticks when the engine first starts. ~Alex
    if(clock_gettime(CLOCK_MONOTONIC, &epoch)) {
        puts("main: Failed to initialize timer");
        glXMakeCurrent(DISPLAY, None, NULL);
        glXDestroyContext(DISPLAY, context);
        XDestroyWindow(DISPLAY, WINDOW);
        XCloseDisplay(DISPLAY);
        return 1;
    }
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
        } else {
            // We'll need to adjust the struct timespec to reflect time since
            // the epoch and not time since the system started. ~Alex
            now.tv_sec -= epoch.tv_sec;
            if(epoch.tv_nsec > now.tv_nsec) {
                now.tv_nsec = 1000000000 + (now.tv_nsec - epoch.tv_nsec);
                now.tv_sec--;
            } else {
                now.tv_nsec -= epoch.tv_nsec;
            }
            expected_tick = (now.tv_sec * BREADBOX_TICKRATE) + (now.tv_nsec / NSEC_PER_TICK);
            if(expected_tick > engine.model.tick) {
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
                    if(expected_tick > engine.model.tick) {
                        printf(
                            "main: Running %u ticks behind! What's going on?\n",
                            expected_tick - engine.model.tick
                        );
                    }
                    msg = BBMSG_TICK;
                    breadbox_publish(&engine, &msg);
                }
            }
        }
        // For those of you wondering, there *is* a reason why I separated the
        // tick trigger from the other events. I consider BBMSG_TICK to be a
        // high priority message compared to the others because of how time
        // sensitive it is. I don't want it to get drowned out by events from X
        // or anything else. Therefore, to prevent potential issues from
        // occuring, I gave it its own dedicated if block above this one. ~Alex
        if(XCheckWindowEvent(DISPLAY, WINDOW, EVENT_MASK, &event)) {
            switch(event.type) {
                case ButtonPress:
                    printf("BUTTON: +%u\n", event.xbutton.button);
                    break;
                case ButtonRelease:
                    printf("BUTTON: -%u\n", event.xbutton.button);
                    break;
                case ConfigureNotify:
                    glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                    break;
                case DestroyNotify:
                    alive = 0;
                    break;
                case KeyPress:
                case KeyRelease:
                    if(event.xkey.type == KeyPress) {
                        printf("KEY: +%u\n", event.xkey.keycode);
                    } else {
                        printf("KEY: -%u\n", event.xkey.keycode);
                    }
                    break;
                default:
                    break;
            }
        } else {
            view(&engine.model);
        }
    }
    glXMakeCurrent(DISPLAY, None, NULL);
    glXDestroyContext(DISPLAY, context);
    // TODO: What if the window hasn't been destroyed yet? ~Alex
    XCloseDisplay(DISPLAY);
    return 0;
}