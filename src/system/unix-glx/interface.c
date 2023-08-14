// Because ANSI C doesn't know what a POSIX is. ~Alex
#define _XOPEN_SOURCE 500

// FOR DEBUGGING PURPOSES
#include <malloc.h>

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include "breadbox.h"

#define EVENT_MASK (ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask)
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

const char *LOG_COLOR[] = {
    "\x1B[90m", // BBLOG_DEBUG
    "\x1B[31m", // BBLOG_ERROR
    "",       // BBLOG_INFO
    "\x1B[33m"  // BBLOG_WARNING
};

const char *LOG_LEVEL[] = {
    "DBG", // BBLOG_DEBUG
    "ERR", // BBLOG_ERROR
    "IFO", // BBLOG_INFO
    "WRN"  // BBLOG_WARNING
};

const char *LOG_SOURCE[] = {
    "ENG", // BBLOG_ENGINE
    "GME", // BBLOG_GAME
    "SYS"  // BBLOG_SYSTEM
};

// Because who doesn't like global variables? *cries in ANSI C* ~Alex
int ALIVE;
Atom *ATOMS;
GLXContext CONTEXT;
Display *DISPLAY;
breadbox_t ENGINE;
struct timespec EPOCH;
breadbox_list_t INPUT_PROGRAM;
Window WINDOW;

// Predefinition for get_subtick here because breadbox_log relies on it. ~Alex
float get_subtick();

// Input interpreter functions
void input_button_press(int id);
void input_button_release(int id);
void input_free(breadbox_list_t *program);
void input_init();
void input_key_press(int id);
void input_key_release(int id);
int input_parse(breadbox_list_t *program, FILE *script);
int input_update(breadbox_list_t *program, breadbox_t *engine);

void breadbox_log(
    breadbox_log_source_t source,
    breadbox_log_level_t level,
    const char *format,
    va_list args
) {
    float current = get_subtick();
    printf(
        "\r%s%13.3f %s %s ",
        LOG_COLOR[level],
        current,
        LOG_SOURCE[source],
        LOG_LEVEL[level]
    );
    vprintf(format, args);
    puts("\x1B[0m");
}

void breadbox_quit() {
    glXMakeCurrent(DISPLAY, None, NULL);
    glXDestroyContext(DISPLAY, CONTEXT);
    // TODO: What if the window hasn't been destroyed yet? ~Alex
    free(ATOMS);
    XCloseDisplay(DISPLAY);
    input_free(&INPUT_PROGRAM);
    breadbox_cleanup(&ENGINE);
    breadbox_model_free(&ENGINE.model);
    exit(ALIVE);
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
        breadbox_error_internal(BBLOG_SYSTEM, "get_subtick: Failed to read monotonic clock!");
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
    if(sig == SIGINT) {
        breadbox_warning_internal(BBLOG_SYSTEM, "interrupt: SIGINT detected! Requesting engine close.");
        ALIVE = 0;
    }
}

int main(void) {
    XEvent event;
    int expected_tick;
    int input_changed = 0;
    FILE *input_script;
    breadbox_message_t msg;
    struct timespec now;
    XVisualInfo *visinfo;
    XWindowAttributes winattr;
    ALIVE = 1;
    // Before we do ANYTHING, we'll put a failsafe in so we can handle
    // interrupts at the very least. ~Alex
    signal(SIGINT, interrupt);
    // Pre-initializing the clock so the timestamps during initialization aren't
    // messed up. ~Alex
    if(clock_gettime(CLOCK_MONOTONIC, &EPOCH)) {
        breadbox_error_internal(BBLOG_SYSTEM, "main: Failed to initialize timer");
        return 1;
    }
    breadbox_model_init(&ENGINE.model);
    breadbox_subscription_init(&ENGINE.subscriptions);
    DISPLAY = XOpenDisplay(NULL);
    if(!DISPLAY) {
        breadbox_error_internal(BBLOG_SYSTEM, "main: Unable to connect to X server!");
        return 1;
    }
    // Locate some atoms for ICCCM since some window managers aren't very
    // straightforward when it comes to closing windows. ~Alex
    ATOMS = malloc(sizeof(Atom) * 2);
    if(!ATOMS) {
        breadbox_error_internal(BBLOG_SYSTEM, "main: Unable to allocate space for atoms!");
        XCloseDisplay(DISPLAY);
        return 1;
    }
    // We cast here because the compiler warns about a constant value becoming a
    // regular value. I do not believe Xlib modifies the value given, so it
    // should be safe to do in this case. If I'm wrong, open an issue and I'll
    // fix this. ~Alex
    if(!XInternAtoms(DISPLAY, (char **)ATOM_NAMES, 2, True, ATOMS)) {
        breadbox_error_internal(BBLOG_SYSTEM, "main: Unable to locate atoms!");
        free(ATOMS);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    visinfo = glXChooseVisual(DISPLAY, 0, (int *)GL_ATTR);
    if(!visinfo) {
        breadbox_error_internal(BBLOG_SYSTEM, "main: No compatible visual found!");
        free(ATOMS);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    if(create_window(visinfo)) {
        breadbox_error_internal(BBLOG_SYSTEM, "main: Unable to create WINDOW!");
        free(ATOMS);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    XMapWindow(DISPLAY, WINDOW);
    XStoreName(DISPLAY, WINDOW, "Breadbox");
    CONTEXT = glXCreateContext(DISPLAY, visinfo, NULL, GL_TRUE);
    if(!CONTEXT) {
        breadbox_error_internal(BBLOG_SYSTEM, "main: Failed to create GL context!");
        XDestroyWindow(DISPLAY, WINDOW);
        free(ATOMS);
        XCloseDisplay(DISPLAY);
        return 1;
    }
    if(!(input_script = fopen("input.fth", "r"))) {
        breadbox_error_internal(BBLOG_SYSTEM, "main: Failed to read input.fth");
        breadbox_quit();
    }
    input_init();
    input_parse(&INPUT_PROGRAM, input_script);
    fclose(input_script);
    // XSelectInput needs to happen after glXCreateContext because OpenGL fails
    // to load otherwise. ~Alex
    XSelectInput(DISPLAY, WINDOW, EVENT_MASK);
    glXMakeCurrent(DISPLAY, WINDOW, CONTEXT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    // TEST LIGHT. TO BE REMOVED LATER. ~Alex
    vec4 TEST_LIGHT[4] = {
        {0.0, 0.0, 0.0, 1.0}, // GL_AMBIENT
        {1.0, 1.0, 1.0, 1.0}, // GL_DIFFUSE
        {0.33, 0.33, 0.33, 1.0}, // GL_SPECULAR
        {1.0, 1.0, 1.0, 1.0}  // GL_POSITION
    };
    glLightfv(GL_LIGHT0, GL_AMBIENT, TEST_LIGHT[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, TEST_LIGHT[1]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, TEST_LIGHT[2]);
    glLightfv(GL_LIGHT0, GL_POSITION, TEST_LIGHT[3]);
    glEnable(GL_LIGHT0);
    // END OF TEST LIGHT
    // NOTE: If the window ever gets resized, then we'll need to run these
    // again. ~Alex
    XGetWindowAttributes(DISPLAY, WINDOW, &winattr);
    glViewport(0, 0, winattr.width, winattr.height);
    ENGINE.subscriptions.height = winattr.height;
    ENGINE.subscriptions.width = winattr.width;
    // Wait until everything else has been initialized before calling the game!
    // ~Alex
    breadbox_init(&ENGINE);
    // We're putting epoch initialization here to make the difference between
    // the epoch and the first timestamp as little as possible so it doesn't
    // complaining about missing ticks when the engine first starts. ~Alex
    if(clock_gettime(CLOCK_MONOTONIC, &EPOCH)) {
        breadbox_error_internal(BBLOG_SYSTEM, "main: Failed to initialize timer");
        breadbox_quit();
    } else {
        breadbox_info_internal(BBLOG_SYSTEM, "main: Initialization complete. Tick values are now real.");
    }
    malloc_stats(); // FOR TESTING PURPOSES
    while(ALIVE) {
        if(clock_gettime(CLOCK_MONOTONIC, &now)) {
            breadbox_error_internal(BBLOG_SYSTEM, "main: Failed to read the monotonic clock! Things might get weird!");
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
            if(expected_tick > ENGINE.subscriptions.tick) {
                if(!++ENGINE.subscriptions.tick) {
                    // If anybody is crazy enough to run the engine for this long,
                    // let me know about it. They're my kind of crazy. ~Alex
                    breadbox_error_internal(
                        BBLOG_SYSTEM,
                        "main: Did you seriously run this game for almost 7 years"
                        "straight? You have my respect. This message will now"
                        "self-destruct. ~Alex"
                    );
                    ALIVE = 0;
                    break;
                } else {
                    if(expected_tick > ENGINE.subscriptions.tick) {
                        if(expected_tick - ENGINE.subscriptions.tick == 1) {
                            breadbox_warning_internal(BBLOG_SYSTEM, "main: Running 1 tick behind! What's going on?");
                        } else {
                            breadbox_warning_internal(
                                BBLOG_SYSTEM,
                                "main: Running %u ticks behind! What's going on?",
                                expected_tick - ENGINE.subscriptions.tick
                            );
                        }
                    }
                    msg = BBMSG_TICK;
                    breadbox_publish(&ENGINE, msg);
                    // Is this the best place for this? ~Alex
                    if(input_changed) {
                        input_update(&INPUT_PROGRAM, &ENGINE);
                        input_changed = 0;
                    }
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
                    input_button_press(event.xbutton.button);
                    input_changed = 1;
                    break;
                case ButtonRelease:
                    input_button_release(event.xbutton.button);
                    input_changed = 1;
                    break;
                // Some window managers aren't nice and decide to simply unmap
                // windows rather than destroy them. In those cases, we'll
                // receive a client message that will tell us that the window
                // has been closed. ~Alex
                case ClientMessage:
                    if(event.xclient.message_type == ATOMS[WM_PROTOCOLS]) {
                        // TODO: Maybe we'll need to handle other similar requests in the future?
                        if((Atom)event.xclient.data.l[0] == ATOMS[WM_DELETE_WINDOW]) {
                            // As you wish! ~Alex
                            ALIVE = 0;
                            XDestroyWindow(DISPLAY, WINDOW);
                        }
                    }
                    break;
                case ConfigureNotify:
                    glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                    ENGINE.subscriptions.height = event.xconfigure.height;
                    ENGINE.subscriptions.width = event.xconfigure.width;
                    break;
                case DestroyNotify:
                    ALIVE = 0;
                    break;
                case KeyPress:
                    input_key_press(event.xkey.keycode);
                    input_changed = 1;
                    break;
                case KeyRelease:
                    input_key_release(event.xkey.keycode);
                    input_changed = 1;
                    break;
                default:
                    break;
            }
        } else {
            view(&ENGINE.model);
        }
    }
    breadbox_quit();
    return 0;
}