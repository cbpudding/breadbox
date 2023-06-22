#include <stdio.h>

#include "breadbox.h"

void breadbox_init(breadbox_t *engine) {
    breadbox_subscribe(&engine->subscriptions, BBMSG_TICK);
    // ...
    breadbox_info("Sandbox started");
}

void breadbox_update(breadbox_t *engine, breadbox_message_t *msg) {
    // ...
}