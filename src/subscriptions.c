#include "breadbox.h"

void breadbox_publish(breadbox_t *engine, breadbox_message_t *msg) {
    switch(*msg) {
        case BBMSG_VIEW:
            breadbox_view(&engine->model);
            break;
        default:
            break;
    }
}