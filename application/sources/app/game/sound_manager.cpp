#include "sound_manager.h"

bool sound_on = true;


void sound_init()
{
    sound_on = true;
}


void sound_enable(bool enable)
{
    sound_on = enable;
}


bool sound_is_enable()
{
    return sound_on;
}


void sound_play(buzzer_sound_t sound)
{
    if(sound_on)
    {
        BUZZER_PlaySound(sound);
    }
}