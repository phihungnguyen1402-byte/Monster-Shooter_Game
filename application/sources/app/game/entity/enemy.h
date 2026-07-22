#pragma once

#include "entity.h"

#define MAX_ENEMIES 6

typedef enum
{
    ENEMY_NORMAL = 0,
    ENEMY_TANK,
    ENEMY_FAST

} enemy_type_t;

typedef struct
{
    entity_t base;

    uint8_t hp;

    enemy_type_t type;

    uint8_t lane;

} enemy_t;

extern enemy_t enemies[MAX_ENEMIES];

void enemy_init();

void enemy_update();

bool enemy_spawn(uint8_t lane, int16_t y, enemy_type_t type);

void enemy_damage(enemy_t* enemy, uint8_t damage);