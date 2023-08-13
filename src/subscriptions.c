#include "breadbox.h"

void breadbox_publish(breadbox_t *engine, breadbox_message_t msg) {
    // If the event bit is set in the mask, fire the update function. ~Alex
    if(engine->subscriptions.mask[msg / 32] & (1 << (msg % 32))) {
        breadbox_update(engine, msg);
    }
}

void breadbox_subscribe(breadbox_subscriptions_t *subs, breadbox_message_t type) {
    // As long as the values passed are actually from the message type enum,
    // this shouldn't overflow. ~Alex
    subs->mask[type / 32] |= 1 << (type % 32);
}

void breadbox_subscription_init(breadbox_subscriptions_t *subs) {
    for(int i = 0; i < 32; i++) {
        subs->axes[i] = 0.0;
    }
    subs->height = 1;
    for(int i = 0; i < 4; i++) {
        subs->mask[i] = 0;
    }
    subs->tick = 0;
    subs->width = 1;
}