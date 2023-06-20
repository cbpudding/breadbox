#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include "breadbox.h"

#define EVENT_MASK ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask
#define NSEC_PER_TICK (1000000000 / BREADBOX_TICKRATE)

extern void view(breadbox_model_t *model);

enum ATOM_INDICES {
    WM_DELETE_WINDOW = 0,
    WM_PROTOCOLS
};

const char *ATOM_NAMES[] = {
    "WM_DELETE_WINDOW",
    "WM_PROTOCOLS"
};

const GLint GL_ATTR[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

// Because who doesn't like global variables? *cries in ANSI C* ~Alex
int ALIVE;
GLXContext CONTEXT;
Display *DISPLAY;
struct timespec EPOCH;
jmp_buf PORTAL;
Window WINDOW;

void breadbox_quit(breadbox_t *engine) {
    // Simple enough on this platform(for now) ~Alex
    ALIVE = 0;
    longjmp(PORTAL, 0);
}

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

float get_subtick() {
    float current = 0.0;
    struct timespec now;
    if(clock_gettime(CLOCK_MONOTONIC, &now)) {
        puts("get_subtick: Failed to read monotonic clock!");
    } else {
        now.tv_sec -= EPOCH.tv_sec;
        if(EPOCH.tv_nsec > now.tv_nsec) {
            now.tv_nsec = 1000000000 + (now.tv_nsec - EPOCH.tv_nsec);
            now.tv_sec--;
        } else {
            now.tv_nsec -= EPOCH.tv_nsec;
        }
        current = (float)(now.tv_sec * BREADBOX_TICKRATE) + ((float)now.tv_nsec / (float)NSEC_PER_TICK);
    }
    return current;
}

void interrupt(int sig) {
    // Just in case the engine locks up on my laptop again, this will at least
    // let me type normally... ~Alex
    puts("interrupt: SIGINT detected! Terminating program gracefully.");
    XAutoRepeatOn(DISPLAY);
    glXMakeCurrent(DISPLAY, None, NULL);
    glXDestroyContext(DISPLAY, CONTEXT);
    XDestroyWindow(DISPLAY, WINDOW);
    XCloseDisplay(DISPLAY);
    exit(1);
}

int main(int argc, char *argv[]) {
    Atom *atoms;
    breadbox_t engine;
    XEvent event;
    int expected_tick;
    breadbox_message_t msg;
    struct timespec now;
    XVisualInfo *visinfo;
    XWindowAttributes winattr;
    ALIVE = 1;
    // Before we do ANYTHING, we'll put a failsafe in so we can handle
    // interrupts at the very least. ~Alex
    signal(SIGINT, interrupt);
    breadbox_model_init(&engine.model);
    breadbox_subscription_init(&engine.subscriptions);
    breadbox_init(&engine);
    DISPLAY = XOpenDisplay(NULL);
    if(!DISPLAY) {
        puts("main: Unable to connect to X server!");
        return 1;
    }
    // Locate some atoms for ICCCM since some window managers aren't very
    // straightforward when it comes to closing windows. ~Alex
    atoms = malloc(sizeof(Atom) * 2);
    if(!atoms) {
        puts("main: Unable to allocate space for atoms!");
        XCloseDisplay(DISPLAY);
        return 1;
    }
    // We cast here because the compiler warns about a constant value becoming a
    // regular value. I do not believe Xlib modifies the value given, so it
    // should be safe to do in this case. If I'm wrong, open an issue and I'll
    // fix this. ~Alex
    if(!XInternAtoms(DISPLAY, (char **)ATOM_NAMES, 2, True, atoms)) {
        puts("main: Unable to locate atoms!");
        free(atoms);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    visinfo = glXChooseVisual(DISPLAY, 0, (int *)GL_ATTR);
    if(!visinfo) {
        puts("main: No compatible visual found!");
        free(atoms);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    if(create_window(visinfo)) {
        puts("main: Unable to create WINDOW!");
        free(atoms);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    XMapWindow(DISPLAY, WINDOW);
    XStoreName(DISPLAY, WINDOW, "Breadbox");
    CONTEXT = glXCreateContext(DISPLAY, visinfo, NULL, GL_TRUE);
    if(!CONTEXT) {
        puts("main: Failed to create GL context!");
        XDestroyWindow(DISPLAY, WINDOW);
        free(atoms);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    // XSelectInput needs to happen after glXCreateContext because OpenGL fails
    // to load otherwise. ~Alex
    XSelectInput(DISPLAY, WINDOW, EVENT_MASK);
    glXMakeCurrent(DISPLAY, WINDOW, CONTEXT);
    glEnable(GL_DEPTH_TEST);
    // NOTE: If the window ever gets resized, then we'll need to run these two
    // again. ~Alex
    XGetWindowAttributes(DISPLAY, WINDOW, &winattr);
    glViewport(0, 0, winattr.width, winattr.height);
    // We don't want to deal with event spam from X so we'll tell it that we're
    // only looking for input changes and we don't want auto-repeating keys. ~Alex
    // TODO: Is there a better way of doing this? This disables auto-repeat for
    // EVERY window on the display. ~Alex
    XAutoRepeatOff(DISPLAY);
    // We're putting epoch initialization here to make the difference between
    // the epoch and the first timestamp as little as possible so it doesn't
    // complaining about missing ticks when the engine first starts. ~Alex
    if(clock_gettime(CLOCK_MONOTONIC, &EPOCH)) {
        puts("main: Failed to initialize timer");
        glXMakeCurrent(DISPLAY, None, NULL);
        glXDestroyContext(DISPLAY, CONTEXT);
        XDestroyWindow(DISPLAY, WINDOW);
        free(atoms);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    // Before we jump into the main loop, let's save the current state of the
    // application so we can come back in case things get crazy. ~Alex
    // "Now you're thinking with portals!" -- Valve probably
    setjmp(PORTAL);
    while(ALIVE) {
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
            now.tv_sec -= EPOCH.tv_sec;
            if(EPOCH.tv_nsec > now.tv_nsec) {
                now.tv_nsec = 1000000000 + (now.tv_nsec - EPOCH.tv_nsec);
                now.tv_sec--;
            } else {
                now.tv_nsec -= EPOCH.tv_nsec;
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
                    ALIVE = 0;
                    break;
                } else {
                    if(expected_tick > engine.model.tick) {
                        printf(
                            "main: Running %u ticks behind! What's going on?\n",
                            expected_tick - engine.model.tick
                        );
                    }
                    msg.type = BBMSG_TICK;
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
                // Some window managers aren't nice and decide to simply unmap
                // windows rather than destroy them. In those cases, we'll
                // receive a client message that will tell us that the window
                // has been closed. ~Alex
                case ClientMessage:
                    if(event.xclient.message_type == atoms[WM_PROTOCOLS]) {
                        // TODO: Maybe we'll need to handle other similar requests in the future?
                        if(event.xclient.data.l[0] == atoms[WM_DELETE_WINDOW]) {
                            // As you wish! ~Alex
                            ALIVE = 0;
                            XDestroyWindow(DISPLAY, WINDOW);
                        }
                    }
                    break;
                case ConfigureNotify:
                    glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                    break;
                case DestroyNotify:
                    ALIVE = 0;
                    break;
                case KeyPress:
                    printf("KEY: +%u\n", event.xkey.keycode);
                    break;
                case KeyRelease:
                    printf("KEY: -%u\n", event.xkey.keycode);
                    break;
                default:
                    break;
            }
        } else {
            view(&engine.model);
        }
    }
    // Reverting the XAutoRepeatOff we ran earlier. ~Alex
    XAutoRepeatOn(DISPLAY);
    glXMakeCurrent(DISPLAY, None, NULL);
    glXDestroyContext(DISPLAY, CONTEXT);
    // TODO: What if the window hasn't been destroyed yet? ~Alex
    free(atoms);
    XCloseDisplay(DISPLAY);
    return 0;
}