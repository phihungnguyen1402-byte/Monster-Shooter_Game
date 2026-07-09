#include "scr_idle.h"

using namespace std;

#define MAX_BALL_DISPLAY (16)
#define BALL_MOVE_STEP	 (2)

class ball {
	// rand from a to b
	// (rand() % (b - a + 1)) + a
public:
	static int total;
	int id, x, y, slope, axis_x, axis_y, radius;

	ball() {
		axis_x = 1;
		axis_y = 1;
		slope  = (rand() % (31)) - 15;
		radius = (rand() % (7)) + 6;
		x	   = radius + (rand() % (LCD_WIDTH - 2 * radius));
		y	   = radius + (rand() % (LCD_HEIGHT - 2 * radius));
	}

	int distance(ball &__ball) {
		uint8_t dx, dy;
		dx = abs(x - __ball.x);
		dy = abs(y - __ball.y);
		return sqrt(dx * dx + dy * dy);
	}

	bool is_hit_to_other(ball &__ball) {
		if ((radius + __ball.radius) <= distance(__ball)) {
			return true;
		}
		else {
			return false;
		}
	}

	void moving() {
		if (axis_x > 0) {
			x = x + BALL_MOVE_STEP;
		}
		else {
			x = x - BALL_MOVE_STEP;
		}

		if (axis_y > 0) {
			y += BALL_MOVE_STEP * atan(slope);
		}
		else {
			y -= BALL_MOVE_STEP * atan(slope);
		}

		if (x > ((LCD_WIDTH - 1) - radius) || x < radius) {
			axis_x = -axis_x;
			if (x < radius) {
				x = radius;
			}
			else if (x > ((LCD_WIDTH - 1) - radius)) {
				x = (LCD_WIDTH - 1) - radius;
			}
		}

		if (y > ((LCD_HEIGHT - 1) - radius) || y < radius) {
			axis_y = -axis_y;
			if (y < radius) {
				y = radius;
			}
			else if (y > ((LCD_HEIGHT - 1) - radius)) {
				y = (LCD_HEIGHT - 1) - radius;
			}
		}
	}
};

static void view_scr_idle();

view_dynamic_t dyn_view_idle = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_idle
};

view_screen_t scr_idle = {
	&dyn_view_idle,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

vector<ball> v_idle_ball;
int ball::total;

static void scr_idle_return_screen() {
	timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);
	SCREEN_BACK();
}

void view_scr_idle() {
    view_render.clear();           // Xóa sạch màn hình 
    view_render.setTextSize(2);    // Chữ to
    view_render.setTextColor(WHITE);
    view_render.setCursor(30, 25); // Canh giữa màn hình
    view_render.print("HELLO HUNG"); // Hiển thị vĩnh viễn
}

void scr_idle_handle(ak_msg_t *msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		if (v_idle_ball.empty()) {
			ball new_ball;
			new_ball.id = ball::total++;
			v_idle_ball.push_back(new_ball);
		}

		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);

		timer_set(AC_TASK_DISPLAY_ID, \
				  AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE, \
				  AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE_INTERAL, \
				  TIMER_PERIODIC);
	} break;

	case AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE: {
		for (unsigned int i = 0; i < v_idle_ball.size(); i++) {
			v_idle_ball[i].moving();
		}
	} break;


	case AC_DISPLAY_BUTON_MODE_PRESSED: {
    APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_PRESSED\n");
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);
    SCREEN_TRAN(scr_peashooter_handle, &scr_peashooter);   // <-- THAY vì scr_idle_return_screen()
} break;

	case AC_DISPLAY_BUTON_DOWN_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_PRESSED\n");
		if (v_idle_ball.size()) {
			ball::total--;
			v_idle_ball.pop_back();
		}

		if (v_idle_ball.empty()) {
			scr_idle_return_screen();
		}
	} break;

	default:
		break;
	}
}
