#include "model.h"

#ifndef SYSTEM_H
#define SYSTEM_H

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

#endif