#ifndef BALL_STRUCT_DEF
#define BALL_STRUCT_DEF

#include <stdint.h>

typedef struct Ball
{		
		// (x,y) represents the middle pixel
		double last_x;
		double last_y;
		double x;			
		double y;
	
		// (v_x, v_y) is the actual velocity vector
		double v_x;		// [v_x] = px / 50ms
		double v_y;		// [v_y] = px / 50ms
		double v_angle;
	
		// flags
		uint8_t showed;
		uint8_t moving;
} Ball;

#endif
