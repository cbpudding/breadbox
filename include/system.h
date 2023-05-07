#include "model.h"

#ifndef SYSTEM_H
#define SYSTEM_H

// This is how the system layer sends events to the engine layer. ~Alex
typedef enum {
    BBMSG_PLACEHOLDER
} breadbox_message_t;

// This function should render the given model so the player can see the
// current state of the game. ~Alex
void breadbox_view(breadbox_model_t *model);

#endif