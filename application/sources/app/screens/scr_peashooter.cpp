#include "../game/sound_manager.h"
#include "scr_idle.h"
#include "../game/assets/sprite_boss.h"
#include "../game/game_config.h"
#include "../game/assets/sprite_enemy.h"
#include "../game/assets/sprite_enemy_tank.h"
#include "../game/assets/sprite_enemy_fast.h"
#include "../game/assets/sprite_player.h"
#include "../game/assets/sprite_player_archer.h"
#include "../game/assets/sprite_player_tank.h"
#include "../game/assets/sprite_bullet_arrow.h"
#include "../game/assets/sprite_bullet_lightning.h"
#include "bullet.h"
#include "enemy.h"
#include "scr_peashooter.h"
#include "task_display.h"
#include "player.h"
#include "game_manager.h"
#include "../game/engine/collision.h"
#include "../game/lane.h"
#include "boss.h"
#include "boss_bullet.h"

using namespace std;

// ====== Cấu hình game ======


#define PLAYER_MOVE_DELAY     2
#define PLAYER_FIRE_DELAY      2
#define ENEMY_SPAWN_PERIOD    12

static bool win_sound_played;
static uint32_t score;
static uint32_t level;
static uint32_t tick_count;
static uint8_t firework_frame;
static bool game_over;
static bool boss_spawned;
static bool boss_dead;
static bool warning;
static uint8_t warning_timer;
static uint8_t end_delay;
static void draw_firework(
    int startX,
    int startY,
    int endX,
    int endY,
    uint8_t frame);
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
    end_delay = 0;
    score      = 0;
    tick_count = 0;
    game_over  = false;
    level = 1;
    boss_spawned = false;
    boss_dead = false;
    win_sound_played = false;
    firework_frame = 0;
    bullet_init();
    enemy_init();
    boss_init();
    boss_bullet_init();
    warning = false;
    warning_timer = 0;  
    
}

// ---------- Bắn đạn ----------
static void peashooter_fire()
{
    player_t *p = player_get();

    if (!bullet_has_free_slot())
    {
        return;
    }

    int bullet_x = lane_to_x(p->lane);

    if(game_get_style() == 0)
    {
        // Fighter
        bullet_x -= 1;
    }
    else if(game_get_style() == 1)
    {
        // Archer
        bullet_x -= 2;
    }
    else if(game_get_style() == 2)
    {
        // Tank
        bullet_x -= 1;
    }

    bullet_spawn(
        bullet_x,
        p->base.y - bullets[0].base.height
    );

    sound_play(BUZZER_SOUND_CLICK);
}

// ---------- Sinh quái mới ----------


static void peashooter_spawn_enemy()
{
    uint8_t lane = rand() % LANE_COUNT;

    enemy_type_t type = ENEMY_NORMAL;


    if(game_is_hard_mode())
    {
        uint8_t r = rand() % 10;


        /*
            HARD MODE

            0-2 : NORMAL 30%
            3-6 : TANK   40%
            7-9 : FAST   30%
        */

        if(r < 3)
        {
            type = ENEMY_NORMAL;
        }
        else if(r < 7)
        {
            type = ENEMY_TANK;
        }
        else
        {
            type = ENEMY_FAST;
        }
    }
    else
    {
        // NORMAL MODE giữ nguyên luật cũ

        if (level >= 3)
        {
            uint8_t r = rand() % 10;

            if (r < 2)
            {
                type = ENEMY_FAST;
            }
            else if (r < 5)
            {
                type = ENEMY_TANK;
            }
        }
        else if (level == 2)
        {
            if ((rand() % 4) == 0)
            {
                type = ENEMY_TANK;
            }
        }
    }


    enemy_spawn(
        lane,
        ENEMY_TOP,
        type
    );
}

// ---------- Kiểm tra va chạm AABB ----------


// ---------- Cập nhật logic 1 tick ----------
static void peashooter_update() {
    if (end_delay > 0)
    {
        end_delay--;
    }
    if (game_over) {
        return;
    }
    player_t *p = player_get();  
    
    tick_count++;

    if (boss_dead)
    {
        firework_frame++;

        if (firework_frame >= 24)
        {
            firework_frame = 0;
        }
    }
    if (warning)
    {

        sound_play(BUZZER_SOUND_3BEEP);
        sound_play(BUZZER_SOUND_3BEEP);
        if (warning_timer > 0)
        {
            warning_timer--;
        }
        else
        {
            warning = false;

            boss_spawn();
            
           
            boss_spawned = true;

            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                enemies[i].base.active = false;
            }
        }

        return;
    }
    uint8_t boss_level = 5;

    if(game_is_hard_mode())
    {
        boss_level = 6;
    }

    if (level >= boss_level &&
        !boss_spawned &&
        !warning)
    {
        warning = true;
        warning_timer = 40;

        return;
    }
    // Di chuyển đạn
    bullet_update();
    enemy_update();
    boss_update();
    boss_bullet_update();
    // Di chuyển quái + kiểm tra va chạm với người chơi
    for (int i = 0; i < MAX_ENEMIES; i++){
        if (!enemies[i].base.active) {
            continue;
        }
        

        // Quái chạm đáy / chạm người chơi -> Game Over
        // Quái đi qua người chơi
        if (enemies[i].base.y + enemies[i].base.height >= LCD_HEIGHT)
        {
            enemies[i].base.active = false;

            if (p->hp > 0)
            {
                p->hp--;
            }

            sound_play(BUZZER_SOUND_3BEEP);

            if (p->hp == 0)
            {
                game_over = true;
                end_delay = 40;
                return;
            }

            continue;
        }
        if (collision_check(
            &enemies[i].base,
            &p->base))
        {
            enemies[i].base.active = false;

            if (p->hp > 0)
            {
                p->hp--;
            }

            sound_play(BUZZER_SOUND_3BEEP);

            if (p->hp == 0)
            {
                game_over = true;
                end_delay = 40;
                return;
            }
        }
    }

    // Kiểm tra đạn trúng quái
    for (int b = 0; b < MAX_BULLETS; b++){
        if (!bullets[b].base.active) {
            continue;
        }
        for (int e = 0; e < MAX_ENEMIES; e++) {
            if (!enemies[e].base.active) {
                continue;
            }
            if (collision_check(
                &bullets[b].base,
                &enemies[e].base)) {
                bullets[b].base.active = false;
                enemy_damage(&enemies[e], 1);

                if (!enemies[e].base.active)
                {
                    score++;
                    level = score / 10 + 1;

                    // Quái chết
                    sound_play(BUZZER_SOUND_BANG);
                }
                else
                {
                    // Đạn trúng nhưng quái chưa chết
                    sound_play(BUZZER_SOUND_CLICK);
                }
                break;
            }
        }
    }
    //=========================
    // Boss Bullet -> Player
    //=========================
    for (int i = 0; i < MAX_BOSS_BULLETS; i++)
    {
        if (!boss_bullets[i].base.active)
        {
            continue;
        }

        if (collision_check(
                &boss_bullets[i].base,
                &p->base))
        {
            boss_bullets[i].base.active = false;

            if (p->hp > 0)
            {
                p->hp--;
            }

            sound_play(BUZZER_SOUND_CLICK);

            if (p->hp == 0)
            {
                game_over = true;
               
                return;
            }
        }
    }
    //=========================
    // Player Bullet -> Boss
    //=========================
    if (boss_spawned)
    {
        boss_t *boss = boss_get();

        if (boss->active)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!bullets[i].base.active)
                {
                    continue;
                }

                if (collision_check(
                        &bullets[i].base,
                        &boss->base))
                {
                    bullets[i].base.active = false;

                    if (boss->hp > 0)
                    {
                        boss->hp--;
                    }

                    // Boss chết
                    if (boss->hp == 0)
                    {
                        boss->active = false;

                        boss_dead = true;

                        score += 100;
                        level++;

                        end_delay = 40;

                        return;
                    }

                    sound_play(BUZZER_SOUND_3BEEP);
                }
            }
        }
    }
    // Sinh quái định kỳ
    if (!boss_spawned)
    {
        uint8_t spawn_period;

        if(game_is_hard_mode())
        {
            // Hard spawn nhanh gấp đôi
            spawn_period =
                (ENEMY_SPAWN_PERIOD / 2) - (level - 1);
        }
        else
        {
            spawn_period =
                ENEMY_SPAWN_PERIOD - (level - 1);
        }

        if (spawn_period < 3)
        {
            spawn_period = 3;
        }

        if ((tick_count % spawn_period) == 0)
        {
            peashooter_spawn_enemy();
        }
    }
}

// ---------- Vẽ màn hình ----------
static void view_scr_peashooter() {
    view_render.clear();

        /*======================
            HUD
    =======================*/

    if (!warning && !game_over && !boss_dead)
    {
            //======================
            // Background stars
            //======================

            view_render.drawPixel(48,16,WHITE);
            view_render.drawPixel(63,24,WHITE);
            view_render.drawPixel(82,18,WHITE);
            view_render.drawPixel(55,35,WHITE);
            view_render.drawPixel(76,42,WHITE);
            view_render.drawPixel(30,28,WHITE);
            view_render.drawPixel(94,28,WHITE);
            view_render.drawPixel(14,16,WHITE);
            view_render.drawPixel(110,16,WHITE);
            view_render.drawPixel(40,40,WHITE);
            view_render.drawPixel(67,4,WHITE);
            view_render.drawPixel(14,38,WHITE);
            view_render.drawPixel(110,38,WHITE);
        view_render.drawLine(
            0,
            HUD_HEIGHT,
            127,
            HUD_HEIGHT,
            WHITE
        );

        view_render.drawRect(
            GAME_BORDER_X,
            GAME_BORDER_Y,
            GAME_BORDER_W,
            GAME_BORDER_H,
            WHITE
        );

        view_render.drawLine(
            GAME_BORDER_X,
            PLAYER_LINE_Y,
            GAME_BORDER_X + GAME_BORDER_W,
            PLAYER_LINE_Y,
            WHITE
        );
    }
    if (game_over) {
    view_render.setTextSize(2);

    view_render.setCursor(8,18);

    view_render.print("GAME OVER!");

    view_render.setTextSize(1);

    view_render.setCursor(44,46);

    view_render.print("Score:");

    view_render.print(score);
        return;
    }
    if (warning)
    {
    view_render.setTextSize(2);

    view_render.setCursor(6,26);

    view_render.print("WARNING!!!");

    view_render.drawLine(10,18,118,18,WHITE);

    view_render.drawLine(10,46,118,46,WHITE);

        return;
    }
    if (boss_dead)
    {

        if(!win_sound_played)
        {
            sound_play(BUZZER_SOUND_HIGHSCORE);

            win_sound_played = true;
        }


        view_render.setTextSize(2);

        view_render.setCursor(22,28);

        view_render.print("YOU WIN");
    draw_firework(
        0,
        0,
        28,
        18,
        firework_frame);

    draw_firework(
        127,
        0,
        99,
        18,
        firework_frame);

    draw_firework(
        0,
        63,
        28,
        46,
        firework_frame);

    draw_firework(
        127,
        63,
        99,
        46,
        firework_frame);

        return;
    }
    // Vẽ người chơi (hình chữ nhật ở đáy)
        player_t *p = player_get();
        boss_t *boss = boss_get();
        const uint8_t *current_player_sprite = player_sprite;

        if(game_get_style() == 1)
        {
            current_player_sprite = archer_player_sprite;
        }
        else if(game_get_style() == 2)
        {
            current_player_sprite = tank_player_sprite;
        }
        APP_PRINT("[DBG] PLAYER STYLE = %d\n", game_get_style());
        view_render.drawBitmap(
            p->base.x,
            p->base.y+2,
            current_player_sprite,
            16,
            16,
            WHITE
        );

    // Vẽ đạn
 
    for (int i = 0; i < MAX_BULLETS; i++) {

        if (!bullets[i].base.active)
            continue;

        if(game_get_style() == 0)
        {
            // Fighter - dùng đạn cũ
            view_render.fillRect(
                bullets[i].base.x,
                bullets[i].base.y,
                bullets[i].base.width,
                bullets[i].base.height,
                WHITE
            );
        }
        else if(game_get_style() == 1)
        {
            // Archer - mũi tên
            view_render.drawBitmap(
                bullets[i].base.x,
                bullets[i].base.y,
                arrow_bullet_sprite,
                8,
                8,
                WHITE
            );
        }
        else if(game_get_style() == 2)
        {
            // Tank - tia sét
            view_render.drawBitmap(
                bullets[i].base.x,
                bullets[i].base.y,
                lightning_bullet_sprite,
                4,
                8,
                WHITE
            );
        }
    }
    //====================
    // Vẽ đạn Boss
    //====================
    for (int i = 0; i < MAX_BOSS_BULLETS; i++)
    {
        if (!boss_bullets[i].base.active)
        {
            continue;
        }

    view_render.fillCircle(
        boss_bullets[i].base.x + 2,
        boss_bullets[i].base.y + 2,
        2,
        WHITE
    );
    }
    //==================
    // Vẽ Enemy
    //==================
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].base.active)
            continue;

        const uint8_t *sprite;

        switch (enemies[i].type)
        {
            case ENEMY_NORMAL:
                sprite = enemy_sprite;
                break;

            case ENEMY_TANK:
                sprite = enemy_tank_sprite;
                break;

            case ENEMY_FAST:
                sprite = enemy_fast_sprite;
                break;

            default:
                sprite = enemy_sprite;
                break;
        }

        view_render.drawBitmap(
            enemies[i].base.x,
            enemies[i].base.y,
            sprite,
            8,
            8,
            WHITE
        );
    }
    //==================
    // Vẽ Boss
    //==================
    if (boss->active)
    {
        view_render.drawBitmap(
            boss->base.x,
            boss->base.y,
            boss_sprite,
            32,
            16,
            WHITE
        );
    }

    //==================
    // Vẽ lane
    //==================
    //if (!boss_spawned)
    //{
     //   for (int i = 1; i < LANE_COUNT; i++)
      //  {
     //       view_render.drawLine(
     //           LANE_START_X + i * LANE_WIDTH,
     //           HUD_HEIGHT + 1,
      //          LANE_START_X + i * LANE_WIDTH,
      //          PLAYER_LINE_Y,
       //         WHITE
     //       );
      //  }
    //}

    
    // Vẽ điểm số ở góc trên
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);

        if (!boss_spawned)
    {
        /* Score */
        view_render.setCursor(2,1);
        view_render.print("SCORE:");
        view_render.print((int)score);

        /* Level */
        view_render.setCursor(55,1);
        view_render.print("LEVEL:");
        view_render.print((int)level);

        /* HP */
        view_render.setCursor(104,1);
        view_render.print("HP:");
        view_render.print((int)player_get()->hp);
    }
    else
    {
    boss_t *boss = boss_get();

    // Chữ BOSS
    view_render.setCursor(2, 1);
    view_render.print("BOSS");

    // Khung thanh máu
    view_render.drawRect(
        30,
        1,
        65,
        6,
        WHITE
    );

    // Độ dài thanh máu
    uint8_t hp_width =
        boss->hp * 63 / boss->max_hp;

    // Thanh máu
    view_render.fillRect(
        31,
        2,
        hp_width,
        4,
        WHITE
    );
    view_render.setCursor(98,1);
    view_render.print("HP:");
    view_render.print(player_get()->hp);
}
}
// ---------- Quay về màn hình trước ----------
static void scr_peashooter_return_screen() {
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_PEASHOOTER_UPDATE);
    SCREEN_BACK();
}
static void draw_firework(
    int startX,
    int startY,
    int endX,
    int endY,
    uint8_t frame)
{
    if(frame < 12)
    {
        int x =
            startX +
            (endX-startX)*frame/12;

        int y =
            startY +
            (endY-startY)*frame/12;

       view_render.drawPixel(x,y,WHITE);
    }
    else
    {
        int x=endX;
        int y=endY;

        int r=(frame-12)+2;

        view_render.drawLine(
            x-r,
            y,
            x+r,
            y,
            WHITE);

        view_render.drawLine(
            x,
            y-r,
            x,
            y+r,
            WHITE);

        view_render.drawLine(
            x-r,
            y-r,
            x+r,
            y+r,
            WHITE);

        view_render.drawLine(
            x-r,
            y+r,
            x+r,
            y-r,
            WHITE);
    }
}
// ---------- Xử lý message ----------
void scr_peashooter_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        case SCREEN_ENTRY: {
            
            APP_DBG_SIG("SCR_PEASHOOTER SCREEN_ENTRY\n");

            game_init();
            
            if(game_is_hard_mode())
            {
                APP_DBG("GAME MODE = HARD\n");
            }
            else
            {
                APP_DBG("GAME MODE = NORMAL\n");
            }
            peashooter_reset();

            timer_set(AC_TASK_DISPLAY_ID,
                    AC_DISPLAY_SHOW_PEASHOOTER_UPDATE,
                    AC_DISPLAY_SHOW_PEASHOOTER_UPDATE_INTERVAL,
                    TIMER_PERIODIC);
        }
        break;
        case AC_DISPLAY_SHOW_PEASHOOTER_UPDATE:
        {
            game_update();

            player_t *p = player_get();

            if (p->move_delay > 0)
            {
                p->move_delay--;
            }

            if (p->fire_delay > 0)
            {
                p->fire_delay--;
            }

            peashooter_update();
        }
        break;

        case AC_DISPLAY_BUTON_UP_PRESSED:
        {
            player_t *p = player_get();

            if (p->move_delay == 0)
            {
                if (p->lane < (LANE_COUNT - 1))
                {
                    p->lane++;
                    p->base.x =
                        lane_to_x(p->lane) - p->base.width / 2;
                    p->move_delay = PLAYER_MOVE_DELAY;
                }
            }

        }

        break;

        case AC_DISPLAY_BUTON_DOWN_PRESSED:
        {
            player_t *p = player_get();

            if (p->move_delay == 0)
            {
                if (p->lane > 0)
                {
                    p->lane--;
                   
                    p->base.x =
                    lane_to_x(p->lane) - p->base.width / 2;
                   p->move_delay = PLAYER_MOVE_DELAY;
                }
            }          
        }
        break;

        case AC_DISPLAY_BUTON_MODE_PRESSED:
        {
            APP_DBG_SIG("SCR_PEASHOOTER AC_DISPLAY_BUTON_MODE_PRESSED\n");

            player_t *p = player_get();

            if (game_over || boss_dead)
            {
                if (end_delay == 0)
                {
                    scr_peashooter_return_screen();
                }

                return;
            }
            else
            {
                if (p->fire_delay == 0)
                {
                    if (bullet_has_free_slot())
                    {
                        peashooter_fire();
                        p->fire_delay = PLAYER_FIRE_DELAY;
                    }
                }
            }
        }
        break;

        default:
            break;
    }
}