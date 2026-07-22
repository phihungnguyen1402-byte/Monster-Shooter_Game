#include "bullet.h"

bullet_t bullets[MAX_BULLETS];

void bullet_init() {

    for (int i = 0; i < MAX_BULLETS; i++) {

        bullets[i].base.type = ENTITY_BULLET;

        bullets[i].base.active = false;

        bullets[i].base.width = 2;
        bullets[i].base.height = 3;

        bullets[i].base.vx = 0;
        bullets[i].base.vy = -4;
    }
}

bool bullet_spawn(int16_t x, int16_t y) {

    for (int i = 0; i < MAX_BULLETS; i++) {

        if (!bullets[i].base.active) {

            bullets[i].base.active = true;

            bullets[i].base.x = x;
            bullets[i].base.y = y;

            return true;
        }
    }

    return false;
}

void bullet_update() {

    for (int i = 0; i < MAX_BULLETS; i++) {

        if (!bullets[i].base.active)
            continue;

        bullets[i].base.y += bullets[i].base.vy;

        if (bullets[i].base.y < 0) {

            bullets[i].base.active = false;
        }
    }
}
bool bullet_has_free_slot()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].base.active)
        {
            return true;
        }
    }

    return false;
}