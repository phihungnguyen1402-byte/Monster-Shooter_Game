#include "game_manager.h"
#include "../screens/scr_idle.h"
#include "player.h"
#include "game_renderer.h"


void game_init()
{
    player_init();
    
}


void game_update()
{
    player_update();
}


void game_draw()
{
    game_renderer_clear();


    player_t *p = player_get();


    game_renderer_draw_entity(&p->base);
}