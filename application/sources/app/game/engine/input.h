#pragma once


typedef struct ak_msg_t ak_msg_t;


void input_handle(ak_msg_t *msg);

bool input_left();

bool input_right();

bool input_fire();