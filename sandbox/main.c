#include <stdio.h>

#include "breadbox.h"

void breadbox_update(breadbox_model_t *model, breadbox_message_t *msg) {
    if(*msg == BBMSG_TICK) {
        printf("Tick %u\r\n", model->tick);
    }
}