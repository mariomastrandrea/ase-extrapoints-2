#include "paddle.h"
#include "../lib_game_display_utilities.h"
#include "../pong_model.h"

extern double PI;

void initialize (Paddle* paddle, uint8_t player)
{
		paddle->min_x = WALL_THICKNESS;
		paddle->max_x = MAX_X - WALL_THICKNESS - PADDLE_WIDTH;
	
		if(player == PLAYER1)
		{
				// setting an outscale value for position 
				paddle->last_x = MAX_X;
				paddle->x 		 = MAX_X;
				paddle->next_x = MAX_X;
	
				paddle->top_y = TOP_Y_PLAYER1_PADDLE;
				paddle->bottom_y = BOTTOM_Y_PLAYER1_PADDLE;
				
				// angles range in [17*PI/6, 31*PI/32]
				paddle->reflection_angle_1 = PI + REFLECTION_ANGLE_UNIT;
				paddle->reflection_angle_2 = 2*PI - REFLECTION_ANGLE_UNIT;
		}
		else if(player == PLAYER2)
		{
				paddle->last_x = (paddle->min_x + paddle->max_x)/2;
				paddle->x 		 = (paddle->min_x + paddle->max_x)/2;
				paddle->next_x = (paddle->min_x + paddle->max_x)/2;
				
				paddle->top_y = TOP_Y_PLAYER2_PADDLE;
				paddle->bottom_y = BOTTOM_Y_PLAYER2_PADDLE;
			
				// angles range in [PI/6, 15*PI/16]
				paddle->reflection_angle_1 = PI - REFLECTION_ANGLE_UNIT;
				paddle->reflection_angle_2 = REFLECTION_ANGLE_UNIT;
		}
		
		// initialize flags
		paddle->to_show = 0;
		paddle->showed = 0;
}

// re-initialize paddle position after one scores
void restart_paddle (Paddle *paddle)
{
		paddle->last_x = paddle->x;
		paddle->x = (paddle->min_x + paddle->max_x)/2;
		
		paddle->to_show = 0;
}

// change not ideal paddle position by means of the current x_value, 
// considering the x_scale and the previous positions
uint8_t set_not_ideal_paddle_position (Paddle* paddle, uint16_t x_value, uint16_t x_scale)
{
		uint16_t new_position = paddle->min_x + x_value * (paddle->max_x - paddle->min_x) / x_scale;
		
		// ****************************************************
		// * To handle potmeter non ideality: 								*
		// *	1) consider only changes by >=10 px;						*
		// *	2) change paddle position only if it maintains 	*
		// *		 roughly the same value for 2 cycles					*
		// ****************************************************
	
		if(abs_distance(new_position, paddle->next_x) < 10 
				&& new_position != paddle->min_x && new_position != paddle->max_x)	// in order to avoid problems at positions 0 and paddle_max_x, always consider this two values 'new' 
		{
					// position not changed significantly since last time
				
					if(paddle->next_x == paddle->x)	
						return 0;		// no changes for a long time -> nothing changes
					
					// position changed only for 1 cycle -> update positions and show new paddle image
					paddle->last_x = paddle->x;
					paddle->x = paddle->next_x;
					// (paddle->next_x remains the same, so it is equal to paddle->x)
					return 1;	// show new paddle
		}
				
		// position changed since last time
		
		// update positions only if paddle has just been rendered, in order to save its position in last_x position
		if(paddle->x == paddle->next_x)
		{
				paddle->last_x = paddle->x; 
				paddle->x = paddle->next_x;
				paddle->next_x = new_position;
		}
		else if (abs_distance(new_position, paddle->x) >= 10)
		// if measured position changed too frequently, update only next x-position, but only if significantly different from current position
				paddle->next_x = new_position;	
		
		return 0;		
}

// change paddle position in the direction of the ball
uint8_t smart_change_paddle_position (Paddle *paddle, Ball *ball)
{
		uint16_t paddle_middle_x = paddle->x + PADDLE_WIDTH/2;
		int next_x;
		int shift = ball->x - paddle_middle_x;
		
		if(shift == 0) return 0;	// paddle and ball are in the same direction
		
		// move paddle towards the ball (at most by PLAYER2_PADDLE_MOVING_UNIT)
		if(shift > PLAYER2_PADDLE_MOVING_UNIT)
				shift = PLAYER2_PADDLE_MOVING_UNIT;
		else if(shift < -PLAYER2_PADDLE_MOVING_UNIT)
				shift = -PLAYER2_PADDLE_MOVING_UNIT;
		
		// compute paddle new position
		next_x = paddle->x + shift;
			
		// new position must be in range [min_x, max_x]
		if(next_x < paddle->min_x)
				next_x = paddle->min_x;
		else if(next_x > paddle->max_x)
				next_x = paddle->max_x;
	
		// change paddle's positions
		paddle->last_x = paddle->x;
		paddle->x = next_x;
			
		return (paddle->x != paddle->last_x);
}

// change paddle position moving it left and rigth at a constant speed, regardless of the ball
uint8_t change_paddle_position (Paddle *paddle)
{
		static uint8_t direction = RIGHT;		// paddle start moving to the right
		int next_x;
		
		if(direction == RIGHT)
		{
				next_x = paddle->x + PLAYER2_PADDLE_MOVING_UNIT;		// move paddle to the right
					
				if(next_x >= paddle->max_x)
				{
						direction = LEFT;			// change direction;
						next_x = paddle->max_x;
				}
		}
		else if(direction == LEFT)
		{
				next_x = paddle->x - PLAYER2_PADDLE_MOVING_UNIT;		// move paddle to the left
				
				if(next_x <= paddle->min_x)
				{
						direction = RIGHT;		// change direction
						next_x = paddle->min_x;
				}
		}
		
		// change paddle's positions
		paddle->last_x = paddle->x;
		paddle->x = next_x;
		
		return (paddle->x != paddle->last_x);
}

void draw_paddle (Paddle* paddle)
{
		uint16_t next_position, last_position, distance, i;
		
		if(!paddle->showed)	// paddle has to be drawn for the first time
		{
				draw_rectangle(paddle->x, paddle->top_y, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_COLOR);
				paddle->showed = 1;
				return;
		}
		
		next_position = paddle->x;
		last_position = paddle->last_x;
			
		distance = abs_distance(next_position, last_position);
	
		if(distance == 0) return;		// no changes in position
		
		// paddle position changed here
	
		if(next_position > last_position) // => paddle moved to the right
		{
				// move paddle to the right one pixel at a time
				for(i=0; i<distance; i++)
				{
						// erase an old vertical line
						draw_rectangle(last_position + i, paddle->top_y, 1, PADDLE_HEIGHT, BACKGROUND_COLOR);
						// draw a new vertical line
						draw_rectangle(last_position + PADDLE_WIDTH + i, paddle->top_y, 1, PADDLE_HEIGHT, PADDLE_COLOR);
				}
		}
		else // <= paddle moved to the left
		{
				// move paddle to the left one pixel at a time
				for(i=0; i<distance; i++)
				{
						// erase an old vertical line
						draw_rectangle(last_position + PADDLE_WIDTH - i - 1, paddle->top_y, 1, PADDLE_HEIGHT, BACKGROUND_COLOR);
						// draw a new vertical line
						draw_rectangle(last_position - i - 1, paddle->top_y, 1, PADDLE_HEIGHT, PADDLE_COLOR);
				}
		}
}

// read and set flags

uint8_t has_to_be_shown (Paddle *paddle)
{
		return paddle->to_show;
}

void show (Paddle *paddle, uint8_t show)
{
		paddle->to_show = show;
}
