#include <stdlib.h>

#include "breadbox.h"

// Appends a node to the list
void breadbox_list_append(breadbox_list_t *list, void *data) {
    breadbox_list_node_t *victim = malloc(sizeof(breadbox_list_node_t));
    victim->data = data;
    victim->last = NULL;
    victim->next = NULL;
    if(!list->head) {
        list->head = victim;
        list->tail = victim;
    } else {
        list->tail->next = victim;
        victim->last = list->tail;
        list->tail = victim;
    }
}

// Gets a node at a specific index
breadbox_list_node_t *breadbox_list_get(breadbox_list_t *list, int i) {
    breadbox_list_node_t *victim = list->head;
    while(victim && i > 0) {
        victim = victim->next;
        i--;
    }
    return victim;
}

// Frees a list and all of its nodes
void breadbox_list_free(breadbox_list_t *list) {
    breadbox_list_node_t *next;
    breadbox_list_node_t *victim = list->head;
    while(victim) {
        next = victim->next;
        free(victim);
        victim = next;
    }
    list->head = NULL;
    list->tail = NULL;
}

// Initializes a brand new list
void breadbox_list_init(breadbox_list_t *list) {
    list->head = NULL;
    list->tail = NULL;
}

// Initializes an iterator for a list
void breadbox_list_iter(breadbox_list_t *list, breadbox_list_iter_t *iter) {
    iter->next = list->head;
}

// Gets the next node of a list from an iterator
breadbox_list_node_t *breadbox_list_next(breadbox_list_iter_t *iter) {
    breadbox_list_node_t *victim = NULL;
    if(iter->next) {
        victim = iter->next;
        iter->next = victim->next;
    }
    return victim;
}

// Removes a node from the list
void breadbox_list_remove(breadbox_list_t *list, breadbox_list_node_t *node) {
    if(node->last) {
        node->last->next = node->next;
    } else {
        list->head = node->next;
    }
    if(node->next) {
        node->next->last = node->last;
    } else {
        list->tail = node->last;
    }
    free(node);
}