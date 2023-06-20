#include "breadbox.h"

void breadbox_publish(breadbox_t *engine, breadbox_message_t *msg) {
    switch(msg->type) {
        case BBMSG_TICK:
            breadbox_update(engine, msg);
            break;
        default:
            break;
    }
}

void breadbox_subscription_init(breadbox_subscriptions_t *subs) {
    for(int i = 0; i < 32; i++) {
        subs->axes[i] = 0.0;
    }
}