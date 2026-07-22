#pragma once

#include "../entity/entity.h"

bool collision_check(
    const entity_t* a,
    const entity_t* b
);
void collision_update();