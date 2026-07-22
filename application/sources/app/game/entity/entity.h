#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ENTITY_NONE = 0,
    ENTITY_PLAYER,
    ENTITY_ZOMBIE,
    ENTITY_BULLET,
    ENTITY_EFFECT
} entity_type_t;

typedef struct {
    entity_type_t type;

    int16_t x;
    int16_t y;

    int16_t vx;
    int16_t vy;

    uint8_t width;
    uint8_t height;

    bool active;
} entity_t;