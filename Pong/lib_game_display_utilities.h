#ifndef GAME_DISPLAY_UTILITIES_DEF
#define GAME_DISPLAY_UTILITIES_DEF

#include "../GLCD/GLCD.h"
#include <math.h>
#include "Paddle/paddle_struct.h"

#define BACKGROUND_COLOR Black

// wall
#define WALL_COLOR Red
#define WALL_THICKNESS 5

// Paddle
#define PADDLE_COLOR Green
#define PADDLE_HEIGHT 10
#define PADDLE_WIDTH 	60		// to be defined
#define PADDLE_BORDER_DISTANCE 32		// distance (in px) from the bottom of the screen
#define TOP_Y_PLAYER1_PADDLE (MAX_Y - PADDLE_BORDER_DISTANCE - PADDLE_HEIGHT)
#define TOP_Y_PLAYER2_PADDLE  PADDLE_BORDER_DISTANCE
#define BOTTOM_Y_PLAYER1_PADDLE (MAX_Y - PADDLE_BORDER_DISTANCE)
#define BOTTOM_Y_PLAYER2_PADDLE (PADDLE_BORDER_DISTANCE + PADDLE_HEIGHT)
#define REFLECTION_ANGLE_UNIT (PI / 8.0)
#define PLAYER2_PADDLE_MOVING_PERIOD 2		// how many cycles must elapse to move player2 paddle
#define PLAYER2_PADDLE_MOVING_UNIT   10

// Ball
#define BALL_SIZE	5
#define BALL_X_0 (MAX_X - 1 - WALL_THICKNESS - (BALL_SIZE/2))	// ball initial x-coordinate
#define BALL_Y_0 (MAX_Y/2)					// ball initial y-coordinate
#define BALL_V	10
#define BALL_ANGLE_0	(3.0 * PI / 4.0)	// down-left
#define BALL_COLOR Green

/* !!! IMPORTANT !!! */
#define FLUIDITY_FACTOR 4		// how many ball squares are drawn between the last position and the new one

// score 
#define SCORE_COLOR  White
#define X_0_PLAYER1_SCORE	 (WALL_THICKNESS + 6)					// (X_0_PLAYER1_SCORE, Y_0_PLAYER1_SCORE) ->    top  left pixel of the player1 score text
#define X_0_PLAYER2_SCORE	 (MAX_X - WALL_THICKNESS - 7) // (X_0_PLAYER2_SCORE, Y_0_PLAYER2_SCORE) -> bottom right pixel of the player2 score text
#define Y_0_PLAYER1_SCORE		160
#define Y_0_PLAYER2_SCORE	 (Y_0_PLAYER1_SCORE + 14)

// texts
#define TEXT_COLOR Cyan

// start 
#define START_TEXT_1ST_LINE "Press KEY1"
#define START_TEXT_2ND_LINE "to start :)"
#define X_0_START_1ST_LINE	((MAX_X/2) - (8*strlen(START_TEXT_1ST_LINE)/2) + 4)
#define X_0_START_2ND_LINE  ((MAX_X/2) - (8*strlen(START_TEXT_2ND_LINE)/2) + 4)
#define Y_0_START_1ST_LINE	(MAX_Y/2 - 10)
#define Y_0_START_2ND_LINE  (MAX_Y/2 + 10)

// restart 
#define RESTART_TEXT_1ST_LINE "Press INT0 and"
#define RESTART_TEXT_2ND_LINE "KEY1 to restart"
#define X_0_RESTART_1ST_LINE	(MAX_X/2 - (8*strlen(RESTART_TEXT_1ST_LINE)/2) + 2)
#define X_0_RESTART_2ND_LINE	(MAX_X/2 - (8*strlen(RESTART_TEXT_2ND_LINE)/2) + 2)
#define Y_0_RESTART_1ST_LINE  (MAX_Y/2 - 10)
#define Y_0_RESTART_2ND_LINE  (MAX_Y/2 + 10)

// pause
#define PAUSE_TEXT "Pause"
#define X_0_PAUSE ((MAX_X/2) - (8*strlen(PAUSE_TEXT)/2))
#define Y_0_PAUSE	 (MAX_Y/2)

// game over (win/lose)
#define LOSE_TEXT_COLOR Yellow 
#define WIN_TEXT_COLOR  Yellow
#define LOSE_TEXT "You lose :("
#define WIN_TEXT	"You win :)"
#define X_0_PLAYER1_LOSE_TEXT ((MAX_X/2) - (8*strlen(LOSE_TEXT)/2) + 2)
#define X_0_PLAYER1_WIN_TEXT	((MAX_X/2) - (8*strlen(WIN_TEXT)/2)  + 2)
#define X_0_PLAYER2_LOSE_TEXT ((MAX_X/2) + (8*strlen(LOSE_TEXT)/2) + 1)
#define X_0_PLAYER2_WIN_TEXT	((MAX_X/2) + (8*strlen(WIN_TEXT)/2)  + 1)
#define Y_0_PLAYER1_TEXT  (MAX_Y - PADDLE_BORDER_DISTANCE - PADDLE_HEIGHT - 40)
#define Y_0_PLAYER2_TEXT	(PADDLE_BORDER_DISTANCE + PADDLE_HEIGHT + 40 + 14)

// functions
void draw_walls (void);
void draw_rectangle (int top_left_x, int top_left_y, int width, int height, uint16_t color);
void draw_centered_square (int C_x, int C_y, uint16_t side, uint16_t color);
int round_double (double num);
uint32_t abs_distance (int a, int b);
int min (int a, int b);

uint8_t is_ball_in_player1_score_text_area  (uint16_t x, uint16_t y);
uint8_t is_ball_in_player2_score_text_area  (uint16_t x, uint16_t y);
uint8_t is_ball_in_paddle_area (double x, double y, Paddle *paddle);

#endif
