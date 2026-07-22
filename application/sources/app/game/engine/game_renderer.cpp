#include "game_renderer.h"

#include "view_render.h"


void game_renderer_clear()
{
    view_render.clear();
}


void game_renderer_draw_entity(entity_t* entity)
{

    if(entity == 0)
        return;


    if(entity->active == false)
        return;


    view_render.drawRect(
        entity->x,
        entity->y,
        entity->width,
        entity->height,
        WHITE
    );
}


void game_renderer_update()
{

}