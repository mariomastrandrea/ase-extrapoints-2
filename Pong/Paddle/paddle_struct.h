#ifndef PADDLE_STRUCT_DEF
#define PADDLE_STRUCT_DEF

#include <stdint.h>

// NOTE: x is the coordinate of the left corner
typedef struct Paddle
{
		// save last, current and next position, in order to handling potmeter non ideality
	  // -> paddle will move only if next_x is similar to current x and they are different from the last one
		uint16_t last_x;
		uint16_t x;
		uint16_t next_x;
	
		uint16_t top_y;
		uint16_t bottom_y;
		
		// flags
		uint8_t to_show;
		uint8_t showed;
		
		// utilities
		uint16_t min_x;
		uint16_t max_x;
		double ball_border_left_x;
		double ball_border_right_x;
		
		// angles range reflected by the paddle
		double reflection_angle_1;
		double reflection_angle_2;
} Paddle;

#endif
