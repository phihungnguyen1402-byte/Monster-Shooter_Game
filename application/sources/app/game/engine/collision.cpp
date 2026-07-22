#include "collision.h"
#include "bullet.h"
#include "enemy.h"
bool collision_check(
    const entity_t* a,
    const entity_t* b
) {

    if (!a->active || !b->active)
        return false;

    return

        (a->x < b->x + b->width)

        &&

        (a->x + a->width > b->x)

        &&

        (a->y < b->y + b->height)

        &&

        (a->y + a->height > b->y);
}