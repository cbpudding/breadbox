#include <stdio.h>

#include "breadbox.h"

void breadbox_publish(breadbox_t *engine, breadbox_message_t *msg) {
    switch(*msg) {
        default:
            breadbox_update(engine, msg);
            break;
    }
}

void breadbox_subscription_init(breadbox_subscriptions_t *subs) {
    for(int i = 0; i < 128; i++) {
        subs->axes[i] = 0.0;
    }
}