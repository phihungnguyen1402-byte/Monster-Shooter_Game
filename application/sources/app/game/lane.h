#pragma once

#include <stdint.h>
#include "game_config.h"

static inline int16_t lane_to_x(uint8_t lane)
{
    return LANE_START_X
         + lane * LANE_WIDTH
         + LANE_WIDTH / 2;
}