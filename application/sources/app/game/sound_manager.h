#pragma once

#include <stdbool.h>
#include "buzzer.h"


void sound_init();

void sound_enable(bool enable);

bool sound_is_enable();

void sound_play(buzzer_sound_t sound);