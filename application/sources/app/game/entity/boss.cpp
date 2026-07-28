#include "../sound_manager.h"
#include "boss.h"
#include "boss_bullet.h"
#include "../engine/game_manager.h"
#include "buzzer.h"
static boss_t boss;
bool game_is_hard_mode();
void boss_init()
{
    boss.base.type = ENTITY_ZOMBIE;

    boss.base.width = 32;
    boss.base.height = 16;

    boss.base.x = 48;
    boss.base.y = -20;

    boss.base.vx = 1;
    boss.base.vy = 1;

    boss.base.active = false;
    boss.active = false;
    if(game_is_hard_mode())
    {
        boss.max_hp = 50;
    }
    else
    {
        boss.max_hp = 30;
    }

    boss.hp = 0;
    boss.fire_delay = 30;
}

void boss_spawn()
{
    boss.active = true;
    boss.base.active = true;

    boss.base.x = 48;
    boss.base.y = -13;

    boss.base.vx = 1;
    boss.base.vy = 1;

    if(game_is_hard_mode())
    {
        boss.max_hp = 50;
    }
    else
    {
        boss.max_hp = 30;
    }

    boss.hp = boss.max_hp;
    boss.fire_delay = 30;
}

void boss_update()
{
    if (!boss.active)
    {
        return;
    }

    // Bay xuống
    if (boss.base.y < 12)
    {
        boss.base.y += boss.base.vy;
        return;
    }

    // Bay ngang
    int speed = 1;

    if(boss.hp <= 10)
    {
        speed = 2;
    }

    boss.base.x += boss.base.vx * speed;

    if (boss.base.x >= 96)
    {
        boss.base.x = 96;
        boss.base.vx = -1;
    }

    if (boss.base.x <= 0)
    {
        boss.base.x = 0;
        boss.base.vx = 1;
    }

    //====================
    // Boss bắn đạn
    //====================
    if (boss.fire_delay > 0)
    {
        boss.fire_delay--;
    }
    else
    {
        //sound_play(BUZZER_SOUND_BANG);

        //====================
        // Boss attack pattern
        //====================

        // Máu nhiều: 1 tia giữa
        if (boss.hp > 20)
        {
            boss_bullet_spawn(
                boss.base.x + 16,
                boss.base.y + 16,
                0,
                1
            );
        }


        // Máu trung bình: 2 tia rộng
        else if (boss.hp > 10)
        {
            boss_bullet_spawn(
                boss.base.x + 8,
                boss.base.y + 16,
                -2,
                1
            );

            boss_bullet_spawn(
                boss.base.x + 24,
                boss.base.y + 16,
                2,
                1
            );
        }


        // Máu thấp: 2 tia rộng + nhanh
        else
        {
            boss_bullet_spawn(
                boss.base.x + 8,
                boss.base.y + 16,
                -2,
                1
            );

            boss_bullet_spawn(
                boss.base.x + 24,
                boss.base.y + 16,
                2,
                1
            );
        }

        // Tốc độ bắn
        if (boss.hp > 20)
        {
            boss.fire_delay = 10;
        }
        else if (boss.hp > 10)
        {
            boss.fire_delay = 14;
        }
        else
        {
            boss.fire_delay = 10;
        }
    }
}
boss_t* boss_get()
{
    return &boss;
}