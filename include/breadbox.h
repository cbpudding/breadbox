#include "resources.h"

#ifndef BREADBOX_H
#define BREADBOX_H

#define BREADBOX_TICKRATE 20

typedef enum {
    // Tell the engine that a tick has passed
    BBMSG_TICK,
    // Tell the engine that it is time to re-render
    BBMSG_VIEW
} breadbox_message_t;

typedef struct {
    // Current game tick
    int tick;
} breadbox_model_t;

typedef struct {
    // ...
} breadbox_subscriptions_t;

typedef struct {
    breadbox_model_t model;
    breadbox_subscriptions_t subscriptions;
} breadbox_t;

// This is the main way the engine is notified of events by the system. ~Alex
void breadbox_publish(breadbox_t *engine, breadbox_message_t *msg);

// This function is how we tell the game what's going on. This should only
// be called by the subscription module or messages might not be routed
// properly. Unless this is called from the subscription service, you should
// probably be using breadbox_publish instead. ~Alex
void breadbox_update(breadbox_model_t *model, breadbox_message_t *msg);

// This function should render the given model so the player can see the
// current state of the game. ~Alex
void breadbox_view(breadbox_model_t *model);

#endif