#include "resources.h"

#ifndef BREADBOX_H
#define BREADBOX_H

#define BREADBOX_TICKRATE 20

typedef struct {
    // Current game tick
    int tick;
} breadbox_model_t;

// This is how the system layer sends events to the engine layer. ~Alex
typedef enum {
    // Tell the engine that a tick has passed
    BBMSG_TICK,
    // Tell the engine that it is time to re-render
    BBMSG_VIEW
} breadbox_message_t;

// This function should render the given model so the player can see the
// current state of the game. ~Alex
void breadbox_view(breadbox_model_t *model);

typedef struct {
    // ...
} breadbox_subscriptions_t;

void breadbox_update(breadbox_model_t *model, breadbox_message_t *msg);

typedef struct {
    breadbox_model_t model;
    breadbox_subscriptions_t subscriptions;
} breadbox_t;

void breadbox_publish(breadbox_t *engine, breadbox_message_t *msg);

#endif