#include <stdio.h>

#include "breadbox.h"

void breadbox_init(breadbox_t *engine) {
    breadbox_resource_t test;
    if(!breadbox_resource_load(&test, "test.bbr")) {
        printf("test.data = %x\n", test.data);
        printf("%s\n", test.data);
        breadbox_resource_unload(&test);
    }
}

void breadbox_update(breadbox_t *engine, breadbox_message_t *msg) {
    // ...
}