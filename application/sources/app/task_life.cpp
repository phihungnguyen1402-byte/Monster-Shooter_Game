#include "ak.h"
#include "task_list.h"
#include "task_life.h"
#include "screen_manager.h"
#include "screens.h"

led_t led_life;

void task_life(ak_msg_t* msg) {
    switch (msg->sig) {
        case AC_LIFE_SYSTEM_CHECK: {
            APP_DBG_SIG("AC_LIFE_SYSTEM_CHECK\n");
            led_toggle(&led_life);
            
            sys_ctrl_soft_watchdog_reset();
            sys_ctrl_independent_watchdog_reset();
        } break;

        default:
            break;
    }
}   