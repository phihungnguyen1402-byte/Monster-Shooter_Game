#include "scr_startup.h"

static void view_scr_startup();

view_dynamic_t dyn_view_startup = {
    { .item_type = ITEM_TYPE_DYNAMIC },
    view_scr_startup
};

view_screen_t scr_startup = {
    &dyn_view_startup,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void view_scr_startup() {
    view_render.clear();
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(38, 25);
    view_render.print("Welcome to ");
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(16, 35);
    view_render.print(" Monster Shooter ");
}

void scr_startup_handle(ak_msg_t* msg) {
    switch (msg->sig) {
        case SCREEN_ENTRY: {
        APP_DBG_SIG("SCR_STARTUP SCREEN_ENTRY\n");
        
        } break;

        case AC_DISPLAY_INITIAL:
        {
            view_render.initialize();

            view_render_display_on();

            SCREEN_TRAN(
                scr_idle_handle,
                &scr_idle
            );
        }
        break;

        default: break;
    }
}
