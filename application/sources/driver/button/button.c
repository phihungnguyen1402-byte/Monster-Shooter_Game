#include "button.h"
#include <stddef.h>

#define BUTTON_REPEAT_TIME 50
#define BUTTON_REPEAT_TIME    50
uint8_t button_init(button_t* button, uint32_t u, uint8_t id, pf_button_ctrl init, pf_button_read read, pf_button_callback callback) {
	button->enable		=	BUTTON_DISABLE;
	button->id			=	id;
	button->counter		=	0;
	button->unit		=	u;
	button->state		=	BUTTON_SW_STATE_RELEASED;
	button->counter_enable = BUTTON_ENABLE;
	button->init		=	init;
	button->read		=	read;
	button->callback	=	callback;

	if (button->init) {
		button->init();
	}
	else {
		return BUTTON_DRIVER_NG;
	}

	if (!button->read) {
		return BUTTON_DRIVER_NG;
	}

	if (!button->callback) {
		return BUTTON_DRIVER_NG;
	}

	return BUTTON_DRIVER_OK;
}

void button_enable(button_t* button) {
	button->enable = BUTTON_ENABLE;
}

void button_disable(button_t* button) {
	button->enable = BUTTON_DISABLE;
}

void button_timer_polling(button_t* button) {
	if(button == NULL)
	{
		return;
	}
	uint8_t hw_button_state;
	if (button->enable == BUTTON_ENABLE) {
		hw_button_state = button->read();

		/* hard button pressed */
		if (hw_button_state == BUTTON_HW_STATE_PRESSED) {

			if (button->counter_enable == BUTTON_ENABLE) {
				/* increase button counter */
				button->counter += button->unit;

		/* nhấn lần đầu */
		if (button->counter >= BUTTON_SHORT_PRESS_MIN_TIME &&
			button->state != BUTTON_SW_STATE_PRESSED)
		{
			button->enable = BUTTON_DISABLE;

			button->state = BUTTON_SW_STATE_PRESSED;

			button->callback(button);

			button->enable = BUTTON_ENABLE;
		}
			}
		}
		/* hard button released */
		else {

			/* reset button */
			button->state = BUTTON_SW_STATE_RELEASED;
			button->counter = 0;
			button->counter_enable = BUTTON_ENABLE;
			button->enable = BUTTON_ENABLE;
		}
	}
}
