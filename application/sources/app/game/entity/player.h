#pragma once

#include "entity.h"


typedef struct {

    entity_t base;

    uint8_t hp;

    uint8_t lane;

    uint8_t move_delay;
    
    uint8_t fire_delay;

} player_t;


void player_init();

void player_update();

player_t* player_get();