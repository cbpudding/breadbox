#ifndef BREADBOX_H
#define BREADBOX_H

#define BREADBOX_TICKRATE 20

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

// Initializes the game, not the engine.
void breadbox_init(breadbox_t *engine);

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

#endif