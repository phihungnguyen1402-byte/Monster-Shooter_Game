#pragma once

#include "entity.h"

#define MAX_BULLETS    5

typedef struct {

    entity_t base;

} bullet_t;

extern bullet_t bullets[MAX_BULLETS];

void bullet_init();
void bullet_update();

bool bullet_spawn(int16_t x, int16_t y);

bool bullet_has_free_slot();