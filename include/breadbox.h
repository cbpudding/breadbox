#include "game.h"
#include "model.h"
#include "subscriptions.h"
#include "system.h"

#ifndef BREADBOX_H
#define BREADBOX_H

typedef struct {
    breadbox_model_t model;
    breadbox_subscriptions_t subscriptions;
} breadbox_t;

#endif