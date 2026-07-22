#include "scr_peashooter_menu.h"

view_dynamic_t dyn_view_peashooter_menu;
view_screen_t scr_peashooter_menu;

static void view_scr_peashooter_menu()
{
    view_render.clear();

    view_render.setTextSize(2);
    view_render.setCursor(10,22);
    view_render.print("MENU");
}

void scr_peashooter_menu_handle(ak_msg_t *msg)
{
    switch(msg->sig)
    {
        case SCREEN_ENTRY:
        {
        }
        break;

        case AC_DISPLAY_SHOW_PEASHOOTER_UPDATE:
        {
        }
        break;

        default:
        break;
    }
}