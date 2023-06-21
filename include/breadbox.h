// I'd like to think that stdarg.h is portable... ~Alex
#include <stdarg.h>

#ifndef BREADBOX_H
#define BREADBOX_H

#define BREADBOX_TICKRATE 20

typedef enum {
    BBLOG_DEBUG,
    BBLOG_ERROR,
    BBLOG_INFO,
    BBLOG_WARNING
} breadbox_log_level_t;

typedef enum {
    BBLOG_ENGINE,
    BBLOG_GAME,
    BBLOG_SYSTEM
} breadbox_log_source_t;

typedef enum {
    // Tell the engine that an axis has been updated
    BBMSG_AXIS0,
    BBMSG_AXIS1,
    BBMSG_AXIS2,
    BBMSG_AXIS3,
    BBMSG_AXIS4,
    BBMSG_AXIS5,
    BBMSG_AXIS6,
    BBMSG_AXIS7,
    BBMSG_AXIS8,
    BBMSG_AXIS9,
    BBMSG_AXIS10,
    BBMSG_AXIS11,
    BBMSG_AXIS12,
    BBMSG_AXIS13,
    BBMSG_AXIS14,
    BBMSG_AXIS15,
    BBMSG_AXIS16,
    BBMSG_AXIS17,
    BBMSG_AXIS18,
    BBMSG_AXIS19,
    BBMSG_AXIS20,
    BBMSG_AXIS21,
    BBMSG_AXIS22,
    BBMSG_AXIS23,
    BBMSG_AXIS24,
    BBMSG_AXIS25,
    BBMSG_AXIS26,
    BBMSG_AXIS27,
    BBMSG_AXIS28,
    BBMSG_AXIS29,
    BBMSG_AXIS30,
    BBMSG_AXIS31,
    // Tell the engine that a tick has passed
    BBMSG_TICK,
    // Tell the engine that a trigger has been fired
    BBMSG_TRIGGER0,
    BBMSG_TRIGGER1,
    BBMSG_TRIGGER2,
    BBMSG_TRIGGER3,
    BBMSG_TRIGGER4,
    BBMSG_TRIGGER5,
    BBMSG_TRIGGER6,
    BBMSG_TRIGGER7,
    BBMSG_TRIGGER8,
    BBMSG_TRIGGER9,
    BBMSG_TRIGGER10,
    BBMSG_TRIGGER11,
    BBMSG_TRIGGER12,
    BBMSG_TRIGGER13,
    BBMSG_TRIGGER14,
    BBMSG_TRIGGER15,
    BBMSG_TRIGGER16,
    BBMSG_TRIGGER17,
    BBMSG_TRIGGER18,
    BBMSG_TRIGGER19,
    BBMSG_TRIGGER20,
    BBMSG_TRIGGER21,
    BBMSG_TRIGGER22,
    BBMSG_TRIGGER23,
    BBMSG_TRIGGER24,
    BBMSG_TRIGGER25,
    BBMSG_TRIGGER26,
    BBMSG_TRIGGER27,
    BBMSG_TRIGGER28,
    BBMSG_TRIGGER29,
    BBMSG_TRIGGER30,
    BBMSG_TRIGGER31
} breadbox_message_type_t;

typedef struct {
    // The type of message being sent
    breadbox_message_type_t type;
    // Details about the message
    union {
        // TODO: Define actual message variants
        int placeholder;
    } data;
} breadbox_message_t;

typedef struct {
    // Current game tick
    int tick;
} breadbox_model_t;

typedef struct {
    // Configurable input axes
    float axes[32];
} breadbox_subscriptions_t;

typedef struct {
    breadbox_model_t model;
    breadbox_subscriptions_t subscriptions;
} breadbox_t;

// Prints a debug message to the log
void breadbox_debug(const char *format, ...);

// Same as above for internal use only!
void breadbox_debug_internal(breadbox_log_source_t source, const char *format, ...);

// Prints an error message to the log
void breadbox_error(const char *format, ...);

// Same as above for internal use only!
void breadbox_error_internal(breadbox_log_source_t source, const char *format, ...);

// Prints an informational message to the log
void breadbox_info(const char *format, ...);

// I think you get the idea at this point. ~Alex
void breadbox_info_internal(breadbox_log_source_t source, const char *format, ...);

// Initializes the game, not the engine.
void breadbox_init(breadbox_t *engine);

// Prints a message to the log. It's recommended to use the debug, error, info,
// and warning functions to preserve your remaining sanity. ~Alex
void breadbox_log(breadbox_log_source_t source, breadbox_log_level_t level, const char *format, va_list args);

// Initializes the model
void breadbox_model_init(breadbox_model_t *model);

// This is the main way the engine is notified of events by the system. ~Alex
void breadbox_publish(breadbox_t *engine, breadbox_message_t *msg);

// This function is used by the game itself to let the platform know that we're
// done and everything can be shut down. ~Alex
void breadbox_quit(breadbox_t *engine);

// Initializes engine subscriptions
void breadbox_subscription_init(breadbox_subscriptions_t *subs);

// This function is how we tell the game what's going on. This should only
// be called by the subscription module or messages might not be routed
// properly. Unless this is called from the subscription service, you should
// probably be using breadbox_publish instead. ~Alex
void breadbox_update(breadbox_t *engine, breadbox_message_t *msg);

// Prints a warning message to the log
void breadbox_warning(const char *format, ...);

// The internal version of breadbox_warning
void breadbox_warning_internal(breadbox_log_source_t source, const char *format, ...);

#endif