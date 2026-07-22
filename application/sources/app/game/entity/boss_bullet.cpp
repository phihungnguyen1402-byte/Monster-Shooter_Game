#include "boss_bullet.h"
#include "task_display.h"
boss_bullet_t boss_bullets[MAX_BOSS_BULLETS];

void boss_bullet_init()
{
    for (int i = 0; i < MAX_BOSS_BULLETS; i++)
    {
        boss_bullets[i].base.type = ENTITY_BULLET;

        boss_bullets[i].base.active = false;

        boss_bullets[i].base.width = 2;
        boss_bullets[i].base.height = 4;

        boss_bullets[i].base.vx = 0;
        boss_bullets[i].base.vy = 2;
    }
}

bool boss_bullet_spawn(
    int16_t x,
    int16_t y,
    int8_t vx,
    int8_t vy
)
{
    for (int i = 0; i < MAX_BOSS_BULLETS; i++)
    {
        if (!boss_bullets[i].base.active)
        {
            boss_bullets[i].base.active = true;

            boss_bullets[i].base.x = x;
            boss_bullets[i].base.y = y;
            
            boss_bullets[i].base.vx = vx;
            boss_bullets[i].base.vy = vy;

            return true;
        }
    }

    return false;
}

void boss_bullet_update()
{
    for (int i = 0; i < MAX_BOSS_BULLETS; i++)
    {
        if (!boss_bullets[i].base.active)
            continue;

        boss_bullets[i].base.x += boss_bullets[i].base.vx;
        boss_bullets[i].base.y += boss_bullets[i].base.vy;

        if (boss_bullets[i].base.y > LCD_HEIGHT ||
            boss_bullets[i].base.x < 0 ||
            boss_bullets[i].base.x > LCD_WIDTH)
        {
            boss_bullets[i].base.active = false;
        }
    }
}