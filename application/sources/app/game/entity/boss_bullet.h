#pragma once

#include "entity.h"

#define MAX_BOSS_BULLETS    4

typedef struct
{
    entity_t base;

} boss_bullet_t;

extern boss_bullet_t boss_bullets[MAX_BOSS_BULLETS];

void boss_bullet_init();

void boss_bullet_update();

bool boss_bullet_spawn(
    int16_t x,
    int16_t y,
    int8_t vx,
    int8_t vy
);