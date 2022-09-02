#ifndef BALL_DEF
#define BALL_DEF

#include <stdint.h>
#include "Ball_struct.h"
#include "../Paddle/paddle_struct.h"

// ball main functions
void initialize_ball (Ball* ball);
uint8_t change_ball_position (Ball* ball);
void draw_ball (Ball* ball);
void move_ball_square (int last_C_x, int last_C_y, int C_x, int C_y);

// ball position and hits controls
uint8_t ball_fell_down (double y);
uint8_t ball_fell_up (double y);
uint8_t ball_hits_right_wall   (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_left_wall	   (double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_paddle_top   (Paddle *paddle, double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_paddle_left  (Paddle *paddle, double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_paddle_right (Paddle *paddle, double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);
uint8_t ball_hits_paddle_bottom(Paddle *paddle, double x, double y, double next_x, double next_y, double *hit_x, double *hit_y);

// ball bounces (changing position and velocity components)
void ball_bounces_horizontally(double *x, double *y, double *next_x, double reflection_x, double reflection_y, double *next_v_x, double *v_angle);
void ball_bounces_vertically	(double *x, double *y, double *next_y, double reflection_x, double reflection_y, double *next_v_y, double *v_angle);
void ball_bounces_on_paddle		(Paddle *paddle, double *x, double *y, double *next_x, double *next_y, double reflection_x, double reflection_y, double *next_v_x, double *next_v_y, double *v_angle);

// read & set flags
void move (Ball *ball, uint8_t moving);
uint8_t has_to_be_moved (Ball *ball);
void show_ball (Ball *ball, uint8_t show);

#endif
