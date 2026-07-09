#include "scr_peashooter.h"
using namespace std;

// ====== Cấu hình game ======
#define PLAYER_Y            (LCD_HEIGHT - 6)   // vị trí y cố định của người chơi (đáy màn hình)
#define PLAYER_WIDTH        (8)
#define PLAYER_HEIGHT       (5)
#define PLAYER_STEP         (3)                // tốc độ di chuyển trái/phải

#define BULLET_WIDTH        (2)
#define BULLET_HEIGHT       (3)
#define BULLET_STEP         (4)                // tốc độ đạn bay lên
#define BULLET_MAX          (3)                // số đạn tối đa cùng lúc trên màn hình

#define ENEMY_WIDTH         (6)
#define ENEMY_HEIGHT        (6)
#define ENEMY_STEP          (1)                // tốc độ quái rơi xuống
#define ENEMY_MAX           (4)                // số quái tối đa cùng lúc
#define ENEMY_SPAWN_PERIOD  (12)               // cứ N tick lại sinh 1 quái mới (nếu còn chỗ)

typedef struct {
    int16_t x, y;
    bool    active;
} bullet_t;

typedef struct {
    int16_t x, y;
    bool    active;
} enemy_t;

static int16_t   player_x;
static bullet_t  bullets[BULLET_MAX];
static enemy_t   enemies[ENEMY_MAX];
static uint32_t  score;
static uint32_t  tick_count;
static bool      game_over;

static void view_scr_peashooter();

view_dynamic_t dyn_view_peashooter = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_peashooter
};

view_screen_t scr_peashooter = {
    &dyn_view_peashooter,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

// ---------- Khởi tạo lại trạng thái game ----------
static void peashooter_reset() {
    player_x   = (LCD_WIDTH / 2) - (PLAYER_WIDTH / 2);
    score      = 0;
    tick_count = 0;
    game_over  = false;

    for (int i = 0; i < BULLET_MAX; i++) {
        bullets[i].active = false;
    }
    for (int i = 0; i < ENEMY_MAX; i++) {
        enemies[i].active = false;
    }
}

// ---------- Bắn đạn ----------
static void peashooter_fire() {
    for (int i = 0; i < BULLET_MAX; i++) {
        if (!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].x = player_x + (PLAYER_WIDTH / 2) - (BULLET_WIDTH / 2);
            bullets[i].y = PLAYER_Y - BULLET_HEIGHT;
            BUZZER_PlaySound(BUZZER_SOUND_3BEEP);
            break;
        }
    }
}

// ---------- Sinh quái mới ----------
static void peashooter_spawn_enemy() {
    for (int i = 0; i < ENEMY_MAX; i++) {
        if (!enemies[i].active) {
            enemies[i].active = true;
            enemies[i].x = rand() % (LCD_WIDTH - ENEMY_WIDTH);
            enemies[i].y = 0;
            break;
        }
    }
}

// ---------- Kiểm tra va chạm AABB ----------
static bool rects_overlap(int16_t ax, int16_t ay, int16_t aw, int16_t ah,
                           int16_t bx, int16_t by, int16_t bw, int16_t bh) {
    return (ax < bx + bw) && (ax + aw > bx) &&
           (ay < by + bh) && (ay + ah > by);
}

// ---------- Cập nhật logic 1 tick ----------
static void peashooter_update() {
    if (game_over) {
        return;
    }

    tick_count++;

    // Di chuyển đạn
    for (int i = 0; i < BULLET_MAX; i++) {
        if (bullets[i].active) {
            bullets[i].y -= BULLET_STEP;
            if (bullets[i].y < 0) {
                bullets[i].active = false;
            }
        }
    }

    // Di chuyển quái + kiểm tra va chạm với người chơi
    for (int i = 0; i < ENEMY_MAX; i++) {
        if (!enemies[i].active) {
            continue;
        }
        enemies[i].y += ENEMY_STEP;

        // Quái chạm đáy / chạm người chơi -> Game Over
        if (enemies[i].y + ENEMY_HEIGHT >= LCD_HEIGHT) {
            game_over = true;
            BUZZER_PlaySound(BUZZER_SOUND_3BEEP);
            return;
        }
        if (rects_overlap(enemies[i].x, enemies[i].y, ENEMY_WIDTH, ENEMY_HEIGHT,
                           player_x, PLAYER_Y, PLAYER_WIDTH, PLAYER_HEIGHT)) {
            game_over = true;
            BUZZER_PlaySound(BUZZER_SOUND_3BEEP);
            return;
        }
    }

    // Kiểm tra đạn trúng quái
    for (int b = 0; b < BULLET_MAX; b++) {
        if (!bullets[b].active) {
            continue;
        }
        for (int e = 0; e < ENEMY_MAX; e++) {
            if (!enemies[e].active) {
                continue;
            }
            if (rects_overlap(bullets[b].x, bullets[b].y, BULLET_WIDTH, BULLET_HEIGHT,
                               enemies[e].x, enemies[e].y, ENEMY_WIDTH, ENEMY_HEIGHT)) {
                bullets[b].active = false;
                enemies[e].active = false;
                score++;
                BUZZER_PlaySound(BUZZER_SOUND_3BEEP);
                break;
            }
        }
    }

    // Sinh quái định kỳ
    if ((tick_count % ENEMY_SPAWN_PERIOD) == 0) {
        peashooter_spawn_enemy();
    }
}

// ---------- Vẽ màn hình ----------
static void view_scr_peashooter() {
    view_render.clear();

    if (game_over) {
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(25, 20);
        view_render.print("GAME OVER");
        view_render.setCursor(20, 35);
        view_render.print("Score: ");
        view_render.print((int)score);
        view_render.setCursor(5, 50);
        view_render.print("Press MODE to exit");
        return;
    }

    // Vẽ người chơi (hình chữ nhật ở đáy)
    view_render.fillRect(player_x, PLAYER_Y, PLAYER_WIDTH, PLAYER_HEIGHT, WHITE);

    // Vẽ đạn
    for (int i = 0; i < BULLET_MAX; i++) {
        if (bullets[i].active) {
            view_render.fillRect(bullets[i].x, bullets[i].y, BULLET_WIDTH, BULLET_HEIGHT, WHITE);
        }
    }

    // Vẽ quái
    for (int i = 0; i < ENEMY_MAX; i++) {
        if (enemies[i].active) {
            view_render.drawRect(enemies[i].x, enemies[i].y, ENEMY_WIDTH, ENEMY_HEIGHT, WHITE);
        }
    }

    // Vẽ điểm số ở góc trên
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(0, 0);
    view_render.print((int)score);
}

// ---------- Quay về màn hình trước ----------
static void scr_peashooter_return_screen() {
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_PEASHOOTER_UPDATE);
    SCREEN_BACK();
}

// ---------- Xử lý message ----------
void scr_peashooter_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        case SCREEN_ENTRY: {
            APP_DBG_SIG("SCR_PEASHOOTER SCREEN_ENTRY\n");
            peashooter_reset();
            timer_set(AC_TASK_DISPLAY_ID,
                      AC_DISPLAY_SHOW_PEASHOOTER_UPDATE,
                      AC_DISPLAY_SHOW_PEASHOOTER_UPDATE_INTERVAL,
                      TIMER_PERIODIC);
        } break;

        case AC_DISPLAY_SHOW_PEASHOOTER_UPDATE: {
            peashooter_update();
        } break;

        case AC_DISPLAY_BUTON_UP_PRESSED: {
            // UP = di chuyển trái
            if (!game_over) {
                player_x -= PLAYER_STEP;
                if (player_x < 0) {
                    player_x = 0;
                }
            }
        } break;

        case AC_DISPLAY_BUTON_DOWN_PRESSED: {
            // DOWN = di chuyển phải
            if (!game_over) {
                player_x += PLAYER_STEP;
                if (player_x > (LCD_WIDTH - PLAYER_WIDTH)) {
                    player_x = LCD_WIDTH - PLAYER_WIDTH;
                }
            }
        } break;

        case AC_DISPLAY_BUTON_MODE_PRESSED: {
            // MODE = bắn đạn, nếu đang Game Over thì thoát ra
            APP_DBG_SIG("SCR_PEASHOOTER AC_DISPLAY_BUTON_MODE_PRESSED\n");
            if (game_over) {
                scr_peashooter_return_screen();
            } else {
                peashooter_fire();
            }
        } break;

        default:
            break;
    }
}