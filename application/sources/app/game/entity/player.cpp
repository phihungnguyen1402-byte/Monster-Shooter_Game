#include "../lane.h"
#include "player.h"
#include "task_display.h"
#include "../assets/sprite_player.h"
#include "../assets/sprite_player_archer.h"
#include "../assets/sprite_player_tank.h"
#include "../../screens/scr_idle.h"
static player_t player;


void player_init()
{
    player.base.type = ENTITY_PLAYER;

    player.base.width = 16;
    player.base.height = 16;

    player.lane = 2;

   player.base.x =
    lane_to_x(player.lane) - player.base.width / 2;

    player.base.y =
        LCD_HEIGHT - player.base.height - 1;

    player.base.vx = 0;
    player.base.vy = 0;

    player.base.active = true;

    player.hp = 5;

    player.move_delay = 0;

    player.fire_delay = 0;
}

void player_update()
{

}


player_t* player_get()
{
    return &player;
}