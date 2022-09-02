#ifndef PADDLE_DEF
#define PADDLE_DEF

#define RIGHT 1
#define LEFT  2

#include <stdint.h>
#include "../Ball/ball_struct.h"
#include "paddle_struct.h"

// paddle main functions
void initialize (Paddle *paddle, uint8_t player);
void restart_paddle (Paddle *paddle);
uint8_t set_not_ideal_paddle_position (Paddle* paddle, uint16_t x_value, uint16_t x_scale);
uint8_t smart_change_paddle_position (Paddle *paddle, Ball *ball);
uint8_t change_paddle_position (Paddle *paddle);
void draw_paddle (Paddle* paddle);

// read & set flags
uint8_t has_to_be_shown (Paddle *paddle);
void show (Paddle *paddle, uint8_t show);

#endif
