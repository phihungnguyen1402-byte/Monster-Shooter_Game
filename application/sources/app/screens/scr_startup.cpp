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
    view_render.setCursor(30, 25);
    view_render.print("HELLO WORLD");
}

void scr_startup_handle(ak_msg_t* msg) {
    switch (msg->sig) {
        case SCREEN_ENTRY: {
    APP_DBG_SIG("SCR_STARTUP SCREEN_ENTRY\n");
    // Bỏ view_render.initialize() ở đây
} break;

case AC_DISPLAY_INITIAL: {
    APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
    //view_render.initialize();
    //view_render_display_on();
    timer_set(AC_TASK_DISPLAY_ID,
              AC_DISPLAY_SHOW_LOGO,
              AC_DISPLAY_STARTUP_INTERVAL,
              TIMER_ONE_SHOT);
} break;

        case AC_DISPLAY_SHOW_LOGO: {
            SCREEN_TRAN(scr_welcome_handle, &scr_welcome);
        } break;

        case AC_DISPLAY_BUTON_MODE_PRESSED: {
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_LOGO);
            SCREEN_TRAN(scr_welcome_handle, &scr_welcome);
        } break;

        default: break;
    }
}
