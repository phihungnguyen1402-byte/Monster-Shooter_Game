#pragma once

#include "entity.h"

typedef struct
{
    entity_t base;

    uint8_t hp;

    uint8_t max_hp;

    uint8_t fire_delay;

    bool active;

} boss_t;

void boss_init();

void boss_spawn();

void boss_update();

boss_t* boss_get();