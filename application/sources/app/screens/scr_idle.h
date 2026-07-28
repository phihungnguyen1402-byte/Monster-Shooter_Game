#ifndef __SCR_IDLE_H__
#define __SCR_IDLE_H__

#include "screens.h"
extern uint8_t game_mode;
extern bool menu_sound_on;
extern view_screen_t scr_idle;

extern bool menu_sound_on;
void scr_idle_handle(ak_msg_t *msg);
bool game_is_hard_mode();
uint8_t game_get_style();
#endif //__SCR_IDLE_H__