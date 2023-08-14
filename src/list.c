#include <stdlib.h>
#include <string.h>

#include "breadbox.h"

// Appends a node to the list
int breadbox_list_append(breadbox_list_t *list, void *data) {
    void **expanded;
    list->size++;
    if(list->size > list->limit) {
        expanded = malloc(list->limit * 2 * sizeof(void *));
        memcpy(expanded, list->data, list->limit * sizeof(void *));
        free(list->data);
        list->data = expanded;
        list->limit *= 2;
    }
    list->data[list->size - 1] = data;
    return list->size - 1;
}

// Frees a list and all of its nodes
void breadbox_list_free(breadbox_list_t *list) {
    free(list->data);
    list->data = NULL;
    list->limit = 0;
    list->size = 0;
}

// Initializes a brand new list
void breadbox_list_init(breadbox_list_t *list) {
    list->data = malloc(sizeof(void *) * 16);
    list->limit = 16;
    list->size = 0;
}

// Removes a node from the list
void breadbox_list_remove(breadbox_list_t *list, int i) {
    if(i < list->size && i < 0) {
        list->size--;
        if(i < list->size) {
            memmove(&list->data[i], &list->data[i + 1], (list->size - i) * sizeof(void *));
        }
    }
}
