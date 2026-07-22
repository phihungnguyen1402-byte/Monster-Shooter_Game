#include "../lane.h"
#include "enemy.h"
#include "task_display.h"

#include "../assets/sprite_enemy.h"
#include "../assets/sprite_enemy_tank.h"
#include "../assets/sprite_enemy_fast.h"

enemy_t enemies[MAX_ENEMIES];

void enemy_init() {

    for (int i = 0; i < MAX_ENEMIES; i++) {

        enemies[i].base.type = ENTITY_ZOMBIE;

        enemies[i].base.active = false;

        enemies[i].base.width = 8;
        enemies[i].base.height = 8;

        enemies[i].base.vx = 0;
        enemies[i].base.vy = 1;

        enemies[i].hp = 1;

        enemies[i].type = ENEMY_NORMAL;
    }
}

bool enemy_spawn(uint8_t lane, int16_t y, enemy_type_t type)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].base.active)
        {
            enemies[i].base.active = true;

            enemies[i].lane = lane;
            enemies[i].type = type;

            enemies[i].base.x =
                lane_to_x(lane) - enemies[i].base.width / 2;
            enemies[i].base.y = y;

            switch (type)
            {
                case ENEMY_NORMAL:
                    enemies[i].hp = 1;
                    enemies[i].base.vy = 1;
                    break;

                case ENEMY_TANK:
                    enemies[i].hp = 2;
                    enemies[i].base.vy = 1;
                    break;

                case ENEMY_FAST:
                    enemies[i].hp = 1;
                    enemies[i].base.vy = 2;
                    break;
            }

            return true;
        }
    }

    return false;
}

void enemy_update() {

    for (int i = 0; i < MAX_ENEMIES; i++) {

        if (!enemies[i].base.active)
            continue;

        enemies[i].base.y += enemies[i].base.vy;

        if (enemies[i].base.y > LCD_HEIGHT) {

            enemies[i].base.active = false;
        }
    }
}
void enemy_damage(enemy_t* enemy, uint8_t damage)
{
    if (!enemy->base.active)
        return;

    if (enemy->hp > damage)
    {
        enemy->hp -= damage;
    }
    else
    {
        enemy->hp = 0;
        enemy->base.active = false;
    }
}