#include "scr_idle.h"
#include "../game/sound_manager.h"
#include <stdlib.h>
#include "scr_peashooter.h"
#include "../game/assets/sprite_player.h"
#include "../game/assets/sprite_enemy.h"
#include "../game/assets/sprite_enemy_fast.h"
#include "../game/assets/sprite_enemy_tank.h"
#include "../game/assets/sprite_demo_tunnel.h"
using namespace std;
static int hard_extra_spawn = 0;
static uint8_t menu_state = 0;
// 0 = Title
// 1 = Demo
// 2 = Menu
static uint8_t menu_index = 0;


bool menu_sound_on = true;
// 0 = Easy
// 1 = Normal
// 2 = Hard
uint8_t game_mode = 1;
uint8_t game_style = 0;
static uint16_t blink = 0;

static int demo_player_x = 56;
static int demo_player_dir = 1;

#define DEMO_ENEMY_COUNT 5

static int demo_enemy_x[DEMO_ENEMY_COUNT] =
{
    44,
    60,
    70,
    52,
    58
};


static int demo_enemy_y[DEMO_ENEMY_COUNT] =
{
    34,
    8,
    22,
    14,
    5
};


static bool demo_enemy_alive[DEMO_ENEMY_COUNT] =
{
    true,
    true,
    true,
    true,
    true
};
static uint8_t demo_enemy_explosion[DEMO_ENEMY_COUNT]
=
{
    0,
    0,
    0,
    0,
    0
};


static uint16_t demo_enemy_respawn[DEMO_ENEMY_COUNT]
=
{
    0,
    0,
    0,
    0,
    0
};
static const uint8_t* demo_enemy_sprite[DEMO_ENEMY_COUNT]
=
{
    enemy_sprite,
    enemy_fast_sprite,
    enemy_tank_sprite,
    enemy_fast_sprite,
    enemy_sprite
};
#define DEMO_BULLET_COUNT 3


static int demo_bullet_x[DEMO_BULLET_COUNT];


static int demo_bullet_y[DEMO_BULLET_COUNT]
=
{
    40,
    25,
    10
};


static bool demo_bullet_active[DEMO_BULLET_COUNT]
=
{
    true,
    true,
    true
};



static void demo_reset_bullet()
{
    for(int i=0;i<DEMO_BULLET_COUNT;i++)
    {
        demo_bullet_x[i] = demo_player_x + 7;
        demo_bullet_y[i] = 40 - i*15;
        demo_bullet_active[i] = true;
    }
}


static void scr_idle_return_screen() {
	timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);
	SCREEN_BACK();
}
static void draw_demo_tunnel()
{
    int left = 32;
    int right = 96;
    ;

    for(int y = 0; y < 64; y++)
    {
        // Vách mở rộng dần
        if((y % 2) == 0)
        {
            left--;
            right++;
        }

        //-----------------------------
        // VÁCH TRÁI
        //-----------------------------

        for(int x = 0; x <= left; x++)
        {
            // Hoa văn kim loại:
            // 4 pixel sáng, 1 pixel tối
            if((x % 5) != 4)
            {
                view_render.drawPixel(x, y, WHITE);
            }
        }

        //-----------------------------
        // VÁCH PHẢI
        //-----------------------------

        for(int x = right; x < 128; x++)
        {
            if((x % 5) != 4)
            {
                view_render.drawPixel(x, y, WHITE);
            }
        }
    }
    //==========================
    // GÂN KIM LOẠI
    //==========================

    for(int y = 8; y < 64; y += 10)
    {
        view_render.drawLine(0, y, left + 2, y, WHITE);
        view_render.drawLine(right - 2, y, 127, y, WHITE);
    }
    //==========================
    // ĐINH TÁN
    //==========================

    for(int y = 6; y < 64; y += 12)
    {
        view_render.drawPixel(6, y, BLACK);
        view_render.drawPixel(12, y, BLACK);
        view_render.drawPixel(115, y, BLACK);
        view_render.drawPixel(121, y, BLACK);
    }
}
static void draw_enemy_explosion(int x, int y, uint8_t frame)
{
    int cx = x + 4;
    int cy = y + 4;


    if(frame > 8)
    {
        // tâm nổ
        view_render.drawPixel(cx,cy,WHITE);

        // 4 hướng
        view_render.drawPixel(cx-1,cy,WHITE);
        view_render.drawPixel(cx+1,cy,WHITE);
        view_render.drawPixel(cx,cy-1,WHITE);
        view_render.drawPixel(cx,cy+1,WHITE);
    }


    else if(frame > 4)
    {
        // bung ra

        view_render.drawPixel(cx-2,cy,WHITE);
        view_render.drawPixel(cx+2,cy,WHITE);

        view_render.drawPixel(cx,cy-2,WHITE);
        view_render.drawPixel(cx,cy+2,WHITE);


        view_render.drawPixel(cx-1,cy-1,WHITE);
        view_render.drawPixel(cx+1,cy+1,WHITE);
    }


    else
    {
        // tàn lửa

        view_render.drawPixel(cx-3,cy-3,WHITE);
        view_render.drawPixel(cx+3,cy+3,WHITE);

        view_render.drawPixel(cx-3,cy+3,WHITE);
        view_render.drawPixel(cx+3,cy-3,WHITE);
    }
}
static void draw_star_cutout(int x, int y)
{
    const uint16_t star[16] =
    {
        0b0000000110000000,
        0b0000000110000000,
        0b0001000110001000,
        0b0011100110011100,
        0b0111111111111110,
        0b0011111111111100,
        0b0001111111111000,
        0b0000111111110000,
        0b0001111111111000,
        0b0011111111111100,
        0b0111111111111110,
        0b0011100110011100,
        0b0001000110001000,
        0b0000000110000000,
        0b0000000110000000,
        0b0000000000000000
    };


    for(int row=0; row<16; row++)
    {
        for(int col=0; col<16; col++)
        {
            if(star[row] & (1 << (15-col)))
            {
                view_render.drawPixel(
                    x+col,
                    y+row,
                    BLACK
                );
            }
        }
    }
}
void view_scr_idle()
{
    view_render.clear();

    if (menu_state == 0)
    {
        //=========================
        // TITLE
        //=========================
        view_render.setTextSize(2);

        view_render.setCursor(10,8);
        view_render.print("MONSTER");

        view_render.setCursor(44,30);
        view_render.print("SHOOTER");

        view_render.setTextSize(1);

        if ((blink / 15) % 2 == 0)
        {
            view_render.setCursor(38,56);
            view_render.print("PRESS MODE");
        }
    }
    else if (menu_state == 1)
    {
        //=========================
        // DEMO
        //=========================
        draw_demo_tunnel();
        //=========================
        // SAO KHOÉT TRONG VÁCH
        //=========================
draw_star_cutout(4,8);

draw_star_cutout(109,8);
        // Stars
        view_render.drawPixel(48,16,WHITE);
        view_render.drawPixel(63,24,WHITE);
        view_render.drawPixel(82,18,WHITE);
        view_render.drawPixel(55,35,WHITE);
        view_render.drawPixel(72,42,WHITE);
        view_render.drawPixel(30,28,WHITE);
        view_render.drawPixel(94,28,WHITE);
        view_render.drawPixel(14,48,WHITE);
        view_render.drawPixel(110,48,WHITE);
        view_render.drawPixel(40,44,WHITE);
        view_render.drawPixel(67,4,WHITE);

        //=========================
        // ĐẠN
        //=========================

        for(int i=0;i<DEMO_BULLET_COUNT;i++)
        {
            if(demo_bullet_active[i])
            {
                view_render.fillRect(
                    demo_bullet_x[i],
                    demo_bullet_y[i],
                    2,
                    5,
                    WHITE
                );
            }
        }
        for(int i=0;i<DEMO_ENEMY_COUNT;i++)
        {

            if(demo_enemy_alive[i])
            {
            int shake = 0;

            if(demo_enemy_explosion[i] > 8)
            {
                shake = (blink % 2) ? 1 : -1;
            }


            view_render.drawBitmap(
                demo_enemy_x[i] + shake,
                demo_enemy_y[i],
                demo_enemy_sprite[i],
                8,
                8,
                WHITE
            );
            }


            // vẽ nổ khi chết
            if(demo_enemy_explosion[i] > 0)
            {
                draw_enemy_explosion(
                    demo_enemy_x[i],
                    demo_enemy_y[i],
                    demo_enemy_explosion[i]
                );
            }

        }
        //=========================
        // PLAYER PLANE
        //=========================

        view_render.drawBitmap(
            demo_player_x,
            48,
            player_sprite,
            16,
            16,
            WHITE
        );

        view_render.setTextSize(1);

        if ((blink / 15) % 2 == 0)
        {
            view_render.setCursor(35,15);
            view_render.print("PRESS MODE");
            
            view_render.setCursor(21,30);
            view_render.print("Monster Shooter");
        }

    }
    else
    {
        //=========================
        // MENU
        //=========================

        view_render.setTextSize(2);

        view_render.setCursor(20,2);
        view_render.print("SETTINGS");
        
        //----------------------------
        // ĐƯỜNG KẺ PHÂN CÁCH
        //----------------------------
        view_render.drawLine(
            0,
            18,
            127,
            18,
            WHITE
        );

        //----------------------------
         view_render.setTextSize(1);
        view_render.setCursor(12,20);

        if(menu_index==0)
            view_render.print(">>>");

        view_render.print(" PLAY");

        //----------------------------

        view_render.setCursor(12,32);

        if(menu_index==1)
            view_render.print(">>>");

        view_render.print(" SOUND:");

        if(sound_is_enable())
        {
            view_render.print("ON");
        }
        else
        {
            view_render.print("OFF");
        }

        //----------------------------
        // MODE
        //----------------------------

        view_render.setCursor(12,44);

        if(menu_index==2)
            view_render.print(">>>");

        view_render.print(" MODE:");

        if(game_mode==0)
        {
            view_render.print("EASY");
        }
        else if(game_mode==1)
        {
            view_render.print("NORMAL");
        }
        else
        {
            view_render.print("HARD");
        }

        //----------------------------
        // STYLE
        //----------------------------

        view_render.setCursor(12,56);

        if(menu_index==3)
            view_render.print(">>>");

        view_render.print(" STYLE:");

        if(game_style==0)
        {
            view_render.print("FIGHTER");
        }
        else if(game_style==1)
        {
            view_render.print("ARCHER");
        }
        else
        {
            view_render.print("TANK");
        }
    }
}
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
void scr_idle_handle(ak_msg_t *msg) {
	switch (msg->sig) {
    case SCREEN_ENTRY:
    {
        sound_enable(menu_sound_on);
        APP_DBG_SIG("SCREEN_ENTRY\n");

        menu_state = 0;
        menu_index = 0;
        demo_player_x = 56;
        demo_player_dir = 1;
        for(int i=0;i<DEMO_ENEMY_COUNT;i++)
        {
            demo_enemy_alive[i]=true;
            demo_enemy_explosion[i]=0;
            demo_enemy_respawn[i]=0;

            if(game_mode == 2)
            {
                demo_enemy_y[i] = -i * 6;
            }
            else
            {
                demo_enemy_y[i] = -i * 12;
            }
        }
        demo_reset_bullet();
        timer_remove_attr(
            AC_TASK_DISPLAY_ID, 
            AC_DISPLAY_SHOW_IDLE
        );

        timer_set(
            AC_TASK_DISPLAY_ID,
            AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE,
            AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE_INTERAL,
            TIMER_PERIODIC
        );

    }
    break;

    case AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE:
    {
    blink++;

    if(menu_state==1)
    {
        for(int i=0;i<DEMO_BULLET_COUNT;i++)
        {
            if(game_mode == 2)
            {
                demo_bullet_y[i]-=3;
            }
            else
            {
                demo_bullet_y[i]-=5;
            }


            if(demo_bullet_y[i]<-6)
            {
                demo_bullet_y[i]=48;
                demo_bullet_x[i]=demo_player_x+7;
            }
        } 
        //--------------------------------
        // Bullet vs Enemy
        //--------------------------------

        for(int b=0;b<DEMO_BULLET_COUNT;b++)
        {
            for(int e=0;e<DEMO_ENEMY_COUNT;e++)
            {
                if(!demo_enemy_alive[e])
                    continue;

                if(
                    demo_bullet_x[b] >= demo_enemy_x[e] &&
                    demo_bullet_x[b] <= demo_enemy_x[e] + 8 &&
                    demo_bullet_y[b] >= demo_enemy_y[e] &&
                    demo_bullet_y[b] <= demo_enemy_y[e] + 8
                )
                {
                    // quái chết
                    demo_enemy_alive[e] = false;


                    // bật hiệu ứng nổ
                    demo_enemy_explosion[e] = 20;


                    // thời gian hồi sinh
                    if(game_mode == 2)
                    {
                        demo_enemy_respawn[e]=15;
                    }
                    else
                    {
                        demo_enemy_respawn[e]=35;
                    }


                    // reset đạn
                    demo_bullet_y[b] = 45;
                    demo_bullet_x[b] = demo_player_x + 7;
                }
            }
        } 
        //--------------------------------
        // Explosion update
        //--------------------------------

        for(int i=0;i<DEMO_ENEMY_COUNT;i++)
        {
            if(demo_enemy_explosion[i] > 0)
            {
                if(blink % 2 == 0)
                {
                    demo_enemy_explosion[i]--;
                }
            }
        }


        //--------------------------------
        // Enemy Respawn
        //--------------------------------

        for(int i=0;i<DEMO_ENEMY_COUNT;i++)
        {
            if(!demo_enemy_alive[i])
            {
                if(demo_enemy_respawn[i]>0)
                {
                    demo_enemy_respawn[i]--;
                }
                else
                {
                    demo_enemy_alive[i]=true;
                    demo_enemy_explosion[i]=0;

                    demo_enemy_x[i] = 26 + (rand()%48);
                    if(game_mode == 2)
                    {
                        demo_enemy_y[i]=-8-i*6;
                    }
                    else
                    {
                        demo_enemy_y[i]=-8-i*10;
                    }
                }
            }
        }  
        // Máy bay qua lại
        demo_player_x += demo_player_dir * 5;

        if(demo_player_x>96)
        {
            demo_player_dir=-1;
        }

        if(demo_player_x<16)
        {
            demo_player_dir=1;
        }
        // Quái rơi xuống

        for(int i=0;i<DEMO_ENEMY_COUNT;i++)
        {
            if(!demo_enemy_alive[i])
                continue;


            if(game_mode == 2)
            {
                demo_enemy_y[i]+=3;
            }
            else
            {
                demo_enemy_y[i]++;
            }


            if(demo_enemy_y[i]>30)
            {
                demo_enemy_y[i]=5;
            }
        }
    }
    }
    break;

    case AC_DISPLAY_BUTON_MODE_PRESSED:
    {
        APP_DBG(
            "[MODE] state=%d index=%d\n",
            menu_state,
            menu_index
        );


        if(menu_state == 0)
        {
            // TITLE -> DEMO
            menu_state = 1;
        }


        else if(menu_state == 1)
        {
            // DEMO -> MENU
            menu_state = 2;
            menu_index = 0;

        }


        else if(menu_state == 2)
        {
            // MENU

            if(menu_index == 0)
            {
                APP_DBG("[START GAME]\n");

                timer_remove_attr(
                    AC_TASK_DISPLAY_ID,
                    AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE
                );


                SCREEN_TRAN(
                    scr_peashooter_handle,
                    &scr_peashooter
                );
            }

            else if(menu_index == 1)
            {
            menu_sound_on = !menu_sound_on;
            sound_enable(menu_sound_on);

            APP_DBG("menu_sound_on = %d\n", menu_sound_on);
            APP_DBG("sound_is_enable = %d\n", sound_is_enable());
            }


            else if(menu_index == 2)
            {
                game_mode++;

                if(game_mode > 2)
                {
                    game_mode = 0;
                }
            }
            else if(menu_index==3)
            {
                game_style++;

                if(game_style>2)
                {
                    game_style=0;
                }
            }
        }
    }
    break;
    case AC_DISPLAY_BUTON_UP_PRESSED:
    {
        if(menu_state==2)
        {
            if(menu_index>0)
            {
                menu_index--;
            }
            else
            {
            menu_index=3;
            }
        }
    }
    break;
    case AC_DISPLAY_BUTON_DOWN_PRESSED:
    {
        if(menu_state==2)
        {
            if(menu_index<3)
            {
                menu_index++;
            }
            else
            {
                menu_index=0;
            }
        }
    }
    break;
	default:
		break;
	}
}
bool game_is_hard_mode()
{
    return (game_mode == 2);
}
uint8_t game_get_style()
{
    return game_style;
}
