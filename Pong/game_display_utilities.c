#include "lib_game_display_utilities.h"
#include <math.h>
#include "Paddle/paddle_struct.h"

extern uint16_t x_f_player1_score;
extern uint16_t x_f_player2_score;

void draw_rectangle (int top_left_x, int top_left_y, int width, int height, uint16_t color)
{
		uint16_t i,j;
			
		// if the rectangle does not fit the screen, draw only the part inside the screen
		
		if(top_left_x < 0)
		{
				width += top_left_x;
				top_left_x = 0;
		}
		
		if(top_left_y < 0)
		{
				height += top_left_y;
				top_left_y = 0;
		}
	
		if(top_left_x + width > MAX_X)
				width = MAX_X - top_left_x;
		
		if(top_left_y + height > MAX_Y)
				height = MAX_Y - top_left_y;
		
		if(width <= 0 || height <= 0) return;		// no area
	
		for(j=0; j<height; j++)
				for(i=0; i<width; i++)
						LCD_SetPoint(top_left_x + i, top_left_y + j, color);
}

// draw a square knowing its center pixels' coordinates (C_x, C_y)
void draw_centered_square (int C_x, int C_y, uint16_t side, uint16_t color)
{
		//draw a square
		draw_rectangle(C_x-(side/2), C_y-(side/2), side, side, color);
}

void draw_walls (void)
{				
		// left wall
		draw_rectangle(0, 0, WALL_THICKNESS, MAX_Y, WALL_COLOR);
		
		// right wall
		draw_rectangle(MAX_X-WALL_THICKNESS, 0, WALL_THICKNESS, MAX_Y, WALL_COLOR);
}

// it computes the floor(x) function of a double and return a uint16_t integer
int round_double (double num)
{	
		return (int)num;
}

// it computes the absolute difference between two uint16_t integers
uint32_t abs_distance (int a, int b)
{
		return a > b ? (a-b) : (b-a);
}

// it computes the minimum between two uint16_t integers
int min (int a, int b)
{
		return a <= b ? a : b;
}

// boolean function that says if (x,y) ball is in the player1 score text area 
uint8_t is_ball_in_player1_score_text_area (uint16_t x, uint16_t y)
{
		return  ((x >= X_0_PLAYER1_SCORE - (int)(BALL_SIZE/2)) && (x < x_f_player1_score +      (int)(BALL_SIZE/2)) &&			
					   (y >= Y_0_PLAYER1_SCORE - (int)(BALL_SIZE/2)) && (y < Y_0_PLAYER1_SCORE + 16 + (int)(BALL_SIZE/2))    );
}

// boolean function that says if (x,y) ball is in the player2 score text area 
uint8_t is_ball_in_player2_score_text_area (uint16_t x, uint16_t y)
{
		return  ((x >  x_f_player2_score -      (int)(BALL_SIZE/2)) && (x <= X_0_PLAYER2_SCORE + (int)(BALL_SIZE/2)) &&			
					   (y >  Y_0_PLAYER2_SCORE - 16 - (int)(BALL_SIZE/2)) && (y <= Y_0_PLAYER2_SCORE + (int)(BALL_SIZE/2))    );
}

// boolean function that says if (x,y) ball is inside the paddle area
uint8_t is_ball_in_paddle_area (double x, double y, Paddle *paddle)
{
		return ((x >= paddle->x     - (int)(BALL_SIZE/2)) && (x < paddle->x + PADDLE_WIDTH + (int)(BALL_SIZE/2)) && 
						(y >= paddle->top_y - (int)(BALL_SIZE/2)) && (y < paddle->bottom_y 	   		 + (int)(BALL_SIZE/2))	  );
}

