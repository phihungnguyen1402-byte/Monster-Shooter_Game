#include "input.h"
#include "message.h"
#include "task_display.h"


static bool left;
static bool right;
static bool fire;



void input_handle(ak_msg_t *msg)
{
    left  = false;
    right = false;
    fire  = false;


    switch(msg->sig)
    {
        case AC_DISPLAY_BUTON_UP_PRESSED:

            left = true;

            break;


        case AC_DISPLAY_BUTON_DOWN_PRESSED:

            right = true;

            break;


        case AC_DISPLAY_BUTON_MODE_PRESSED:

            fire = true;

            break;


        default:

            break;
    }
}



bool input_left()
{
    return left;
}



bool input_right()
{
    return right;
}



bool input_fire()
{
    return fire;
}