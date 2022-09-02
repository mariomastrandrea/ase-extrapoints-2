/******************************************************
*				NOTE: axis x and y directions below
*	 	|		
*	 -0-------->	x-axis
*		|	
*		|
*		|	
*		v											unit of measure is pixel (px)
*			y-axis
*
*	This means that angles are defined *clockwise* starting from x-axis origin
*
******************************************************/

#include <stdio.h>
#include <string.h>
#include "pong_model.h"
#include "Paddle/paddle.h"
#include "Ball/Ball.h"
#include "lib_game_display_utilities.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../button_EXINT/button.h"
#include "lib_game_sound_utilities.h"

// utility variables
double PI;
uint16_t x_f_player1_score;
uint16_t x_f_player2_score;
uint8_t last_winner;

// PONG game variables
Ball ball;
Paddle player1_paddle;
Paddle player2_paddle;
uint32_t player1_score = 0;
uint32_t player2_score = 0;


void initialize_game_field (void)
{		
		// disable pause button
		disable_button(2);
	
		// compute PI
		PI = acos(-1);
		
		// clear the display
		LCD_Clear(BACKGROUND_COLOR);
				
		// initialize player1's paddle 
		initialize(&player1_paddle, PLAYER1);
		initialize(&player2_paddle, PLAYER2);
		
		// initialize & show players' scores
		player1_score = 0;
		player2_score = 0;
		display_score(PLAYER1);
		display_score(PLAYER2);
}

// button functions

void reset_game (void) 		// (INT0 button)
{
		uint32_t player1_last_score;
		uint32_t player2_last_score;

		// disable the reset button
		disable_button(0);
		
		// clear old ball, if present from last game
		disable_timer(1);
		draw_centered_square(ball.x, ball.y, BALL_SIZE, BACKGROUND_COLOR);
		
		// reset players' scores
		player1_last_score = player1_score;
		player2_last_score = player2_score;
		player1_score = 0;
		player2_score = 0;
		display_new_score(PLAYER1, player1_last_score, player1_score);
		display_new_score(PLAYER2, player2_last_score, player2_score);
		x_f_player1_score = X_0_PLAYER1_SCORE + 8;
		x_f_player2_score = X_0_PLAYER2_SCORE - 8;
		
		clear_won(last_winner);		
		
		// re-initialize and show player2's paddle
		restart_paddle(&player2_paddle);
		draw_paddle(&player2_paddle);
				
		// show player1's paddle 	
		show(&player1_paddle, 1);
		enable_timer(1);
		
		// show start text
		GUI_Text_no_background(X_0_START_1ST_LINE, Y_0_START_1ST_LINE, (uint8_t*)START_TEXT_1ST_LINE, TEXT_COLOR);
		GUI_Text_no_background(X_0_START_2ND_LINE, Y_0_START_2ND_LINE, (uint8_t*)START_TEXT_2ND_LINE, TEXT_COLOR);
		
		// re-draw walls and ball 
		initialize_ball(&ball);
		draw_walls();
			
		// enable start button
		enable_button(1);
}

void start_game (void)		// (KEY1 button)
{
		// clear start text
		GUI_Text_no_background(X_0_START_1ST_LINE, Y_0_START_1ST_LINE, (uint8_t*)START_TEXT_1ST_LINE, BACKGROUND_COLOR);
		GUI_Text_no_background(X_0_START_2ND_LINE, Y_0_START_2ND_LINE, (uint8_t*)START_TEXT_2ND_LINE, BACKGROUND_COLOR);
		
		// start moving player2's paddle
		show(&player2_paddle, 1);

		// start moving ball
		move(&ball, 1);
	
		// enable pause & reset button
		enable_button(2);
		enable_button(0);
}

// wait and restart game after new score
void wait_and_restart (void)
{
		// stop the ball
		move(&ball, 0);
		
		// wait a moment before restart
		reset_timer(3);
		enable_timer(3);
}

void pause (void)				// (KEY2 button)
{
		// write pause text
		GUI_Text_no_background(X_0_PAUSE, Y_0_PAUSE, (uint8_t*)PAUSE_TEXT, TEXT_COLOR);
		
		// stop ball & paddles
		disable_timer(1);
		
		play_pause_note();
}

void clear_pause(void)	// (KEY2 button)
{
		// clear pause text
		GUI_Text_no_background(X_0_PAUSE, Y_0_PAUSE, (uint8_t*)PAUSE_TEXT, BACKGROUND_COLOR);
		
		// move ball & paddles
		enable_timer(1);
	
		play_pause_note();
}

// scores management

// increase player's score by one; return 1 if player won, 0 otherwise
uint8_t increase_score (uint8_t player)
{
		uint32_t player_last_score, *player_score;
	
		switch(player)
		{
				case PLAYER1:
						player_last_score = player1_score;
						player_score = &player1_score;
						break;
					
				case PLAYER2:
						player_last_score = player2_score;
						player_score = &player2_score;
						break;
				
				default: return 0;
		}
		
		// increase player's score by 1
		(*player_score) += 1;
		
		// display player's new score
		display_new_score(player, player_last_score, *player_score);
		
		// check if player won
		if(*player_score < WIN_SCORE) return 0;		// -> no win
		
		won(player);
		return 1;
}

void display_new_score (uint8_t player, uint32_t last_score, uint32_t new_score)
{
		char last_score_str[10], new_score_str[10];
		
		if(last_score == new_score) return;		// score did not change
		
		// convert int to string
		sprintf(last_score_str, "%d", last_score);
		sprintf(new_score_str , "%d", new_score);
		
		switch(player)
		{
				case PLAYER1:
						// update x_f_player1_score
						x_f_player1_score = X_0_PLAYER1_SCORE + 8*strlen(new_score_str);
					
						// clear only old score text's pixels
						GUI_Text_no_background(X_0_PLAYER1_SCORE, Y_0_PLAYER1_SCORE, (uint8_t*)last_score_str, BACKGROUND_COLOR);
						//  draw only new score text's pixels
						GUI_Text_no_background(X_0_PLAYER1_SCORE, Y_0_PLAYER1_SCORE, (uint8_t*)new_score_str , SCORE_COLOR);
						
						break;
				
				case PLAYER2:
						// update x_f_player2_score
						x_f_player2_score = X_0_PLAYER2_SCORE - 8*strlen(new_score_str);
						
						// clear only (reversed) old score text's pixels
						GUI_Text_no_background_reversed(X_0_PLAYER2_SCORE, Y_0_PLAYER2_SCORE, (uint8_t*)last_score_str, BACKGROUND_COLOR);
						//  draw only (reversed) new score text's pixels
						GUI_Text_no_background_reversed(X_0_PLAYER2_SCORE, Y_0_PLAYER2_SCORE, (uint8_t*)new_score_str , SCORE_COLOR);
						
						break;
				
				default: return;
		}
}

void display_score (uint8_t player)
{
		char score_str[10];
		
		switch(player)
		{
				case PLAYER1:
						// convert int to string
						sprintf(score_str, "%d", player1_score);
				
						// draw only score text's pixels
						GUI_Text_no_background(X_0_PLAYER1_SCORE, Y_0_PLAYER1_SCORE, (uint8_t*)score_str, SCORE_COLOR);
				
						break;
				
				case PLAYER2:
						// convert int to string
						sprintf(score_str, "%d", player2_score);
				
						// draw only (reversed) score text's pixels
						GUI_Text_no_background_reversed(X_0_PLAYER2_SCORE, Y_0_PLAYER2_SCORE, (uint8_t*)score_str, SCORE_COLOR);
				
						break;
					
				default: return;
		}
}

// game over management

void won (uint8_t player)
{						
		// disable start & pause button
		disable_button(1);
		disable_button(2);
	
		last_winner = player;
	
		switch(player)
		{
				case PLAYER1:
						// write win text
						GUI_Text_no_background(X_0_PLAYER1_WIN_TEXT, Y_0_PLAYER1_TEXT, (uint8_t*)WIN_TEXT, WIN_TEXT_COLOR);
						// write (reversed) lose text
						GUI_Text_no_background_reversed(X_0_PLAYER2_LOSE_TEXT, Y_0_PLAYER2_TEXT, (uint8_t*)LOSE_TEXT, LOSE_TEXT_COLOR);
						break;
		
				case PLAYER2:				
						// write (reversed) win text
						GUI_Text_no_background_reversed(X_0_PLAYER2_WIN_TEXT, Y_0_PLAYER2_TEXT, (uint8_t*)WIN_TEXT, WIN_TEXT_COLOR);
						// write lose text
						GUI_Text_no_background(X_0_PLAYER1_LOSE_TEXT, Y_0_PLAYER1_TEXT, (uint8_t*)LOSE_TEXT, LOSE_TEXT_COLOR);
						break;
				
				default: return;
		}
		
		// write restart text
		GUI_Text_no_background(X_0_RESTART_1ST_LINE, Y_0_RESTART_1ST_LINE, (uint8_t*)RESTART_TEXT_1ST_LINE, TEXT_COLOR);
		GUI_Text_no_background(X_0_RESTART_2ND_LINE, Y_0_RESTART_2ND_LINE, (uint8_t*)RESTART_TEXT_2ND_LINE, TEXT_COLOR);
		
		// stop moving ball & paddle
		disable_timer(1);
}

void clear_won (uint8_t player)
{				
		switch(player)
		{
				case PLAYER1:
						// clear win text
						GUI_Text_no_background(X_0_PLAYER1_WIN_TEXT, Y_0_PLAYER1_TEXT, (uint8_t*)WIN_TEXT, BACKGROUND_COLOR);
						// clear (reversed) lose text
						GUI_Text_no_background_reversed(X_0_PLAYER2_LOSE_TEXT, Y_0_PLAYER2_TEXT, (uint8_t*)LOSE_TEXT, BACKGROUND_COLOR);
						break;
		
				case PLAYER2:				
						// clear (reversed) win text
						GUI_Text_no_background_reversed(X_0_PLAYER2_WIN_TEXT, Y_0_PLAYER2_TEXT, (uint8_t*)WIN_TEXT, BACKGROUND_COLOR);
						// clear lose text
						GUI_Text_no_background(X_0_PLAYER1_LOSE_TEXT, Y_0_PLAYER1_TEXT, (uint8_t*)LOSE_TEXT, BACKGROUND_COLOR);
						break;
				
				default: return;
		}
		
		// clear restart text
		GUI_Text_no_background(X_0_RESTART_1ST_LINE, Y_0_RESTART_1ST_LINE, (uint8_t*)RESTART_TEXT_1ST_LINE, BACKGROUND_COLOR);
		GUI_Text_no_background(X_0_RESTART_2ND_LINE, Y_0_RESTART_2ND_LINE, (uint8_t*)RESTART_TEXT_2ND_LINE, BACKGROUND_COLOR);
}

