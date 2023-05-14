#include "resources.h"

#ifndef BREADBOX_H
#define BREADBOX_H

#define BREADBOX_TICKRATE 20

typedef enum {
    // Tell the engine that a tick has passed
    BBMSG_TICK
} breadbox_message_t;

typedef struct {
    // Current game tick
    int tick;
} breadbox_model_t;

typedef struct {
    // Configurable input axes
    float axes[128];
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

// Initializes engine subscriptions
void breadbox_subscription_init(breadbox_subscriptions_t *subs);

// This function is how we tell the game what's going on. This should only
// be called by the subscription module or messages might not be routed
// properly. Unless this is called from the subscription service, you should
// probably be using breadbox_publish instead. ~Alex
void breadbox_update(breadbox_t *model, breadbox_message_t *msg);

#endif