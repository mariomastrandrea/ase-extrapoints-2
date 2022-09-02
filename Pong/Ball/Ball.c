#include "Ball.h"
#include "../lib_game_display_utilities.h"
#include "../lib_game_sound_utilities.h"
#include "../../timer/timer.h"
#include "../pong_model.h"
#include "../Paddle/paddle.h"

extern double PI;

double ball_min_x = WALL_THICKNESS + (int)(BALL_SIZE/2);
double ball_max_x = MAX_X - WALL_THICKNESS - ((int)(BALL_SIZE/2));

double wall_min_y = -(int)(BALL_SIZE/2);
double wall_max_y = MAX_Y + (int)(BALL_SIZE/2);

extern Paddle player1_paddle;
extern Paddle player2_paddle;


void initialize_ball (Ball* ball)
{	
		ball->last_x = (double)(MAX_X + BALL_SIZE);		// outscale value
		ball->last_y = (double)(MAX_Y + BALL_SIZE);		// outscale value
		ball->x 		 = (double) BALL_X_0;
		ball->y 		 = (double) BALL_Y_0;

		ball->v_x = BALL_V * cos(BALL_ANGLE_0);
		ball->v_y = BALL_V * sin(BALL_ANGLE_0);
		ball->v_angle = BALL_ANGLE_0;
	
		// initialize flags
		ball->moving = 0;
		ball->showed = 0;
	
		draw_ball(ball);
}

uint8_t change_ball_position (Ball* ball)
{
		double x, y, next_x, next_y, next_v_x, next_v_y, hit_x, hit_y;
		uint8_t won;
		
		// compute next (x,y) position components
		next_x = ball->x + ball->v_x;
		next_y = ball->y + ball->v_y;
		
		// save temporary values
		x = ball->x;
		y = ball->y;
		next_v_x = ball->v_x;
		next_v_y = ball->v_y;
		
		if(ball_fell_down(y))
		{	
				// PLAYER2 scores
				play_new_point_note();
				won = increase_score(PLAYER2);
				
				if(!won)
					wait_and_restart();
			
				return 0;
		}
		else if(ball_fell_up(y))
		{
				// PLAYER1 scores
				play_new_point_note();
				won = increase_score(PLAYER1);
			
				if(!won)
					wait_and_restart();
				
				return 0;
		}
		
		// (multiple) hits controls 
	
		if(ball_hits_left_wall(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
			
				if(ball_hits_paddle_top(&player1_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_on_paddle(&player1_paddle, &x, &y, &next_x, &next_y, hit_x, hit_y, &next_v_x, &next_v_y, &(ball->v_angle)); 
				}
				else if(ball_hits_paddle_bottom(&player2_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_on_paddle(&player2_paddle, &x, &y, &next_x, &next_y, hit_x, hit_y, &next_v_x, &next_v_y, &(ball->v_angle)); 
				}
				else if(ball_hits_paddle_left(&player1_paddle, x, y, next_x, next_y, &hit_x, &hit_y) ||
								ball_hits_paddle_left(&player2_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				}
		}
		else if(ball_hits_right_wall(x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
			
				if(ball_hits_paddle_top(&player1_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_on_paddle(&player1_paddle, &x, &y, &next_x, &next_y, hit_x, hit_y, &next_v_x, &next_v_y, &(ball->v_angle)); 
				}
				else if(ball_hits_paddle_bottom(&player2_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_on_paddle(&player2_paddle, &x, &y, &next_x, &next_y, hit_x, hit_y, &next_v_x, &next_v_y, &(ball->v_angle)); 
				}
				else if(ball_hits_paddle_right(&player1_paddle, x, y, next_x, next_y, &hit_x, &hit_y) ||
								ball_hits_paddle_right(&player2_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				}
		}
		else if(ball_hits_paddle_top(&player1_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_on_paddle(&player1_paddle, &x, &y, &next_x, &next_y, hit_x, hit_y, &next_v_x, &next_v_y, &(ball->v_angle)); 
			
				if(ball_hits_right_wall(x, y, next_x, next_y, &hit_x, &hit_y) ||
					 ball_hits_left_wall (x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				}
		}
		else if(ball_hits_paddle_left(&player1_paddle, x, y, next_x, next_y, &hit_x, &hit_y) ||
						ball_hits_paddle_left(&player2_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
			
				if(ball_hits_left_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				}
		}
		else if(ball_hits_paddle_right(&player1_paddle, x, y, next_x, next_y, &hit_x, &hit_y) || 
						ball_hits_paddle_right(&player2_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
			
				if(ball_hits_right_wall(x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				}
		}
		else if(ball_hits_paddle_bottom(&player2_paddle, x, y, next_x, next_y, &hit_x, &hit_y))
		{
				ball_bounces_on_paddle(&player2_paddle, &x, &y, &next_x, &next_y, hit_x, hit_y, &next_v_x, &next_v_y, &(ball->v_angle)); 
			
				if(ball_hits_right_wall(x, y, next_x, next_y, &hit_x, &hit_y) || 
					 ball_hits_left_wall (x, y, next_x, next_y, &hit_x, &hit_y))
				{
						ball_bounces_horizontally(&x, &y, &next_x, hit_x, hit_y, &next_v_x, &(ball->v_angle));
				}
		}
		
		// ball info updates
		
		ball->last_x = ball->x;
		ball->last_y = ball->y;
		ball->x = next_x;
		ball->y = next_y;
		ball->v_x = next_v_x;
		ball->v_y = next_v_y;
		
		return (ball->last_x != ball->x || ball->last_y != ball->y);
}

void draw_ball (Ball* ball)
{
		uint16_t C_x, C_y, i;
		double t = 0;
		double last_temp_x, last_temp_y, temp_x, temp_y;
		
		if(!ball->showed) 	// for the first time, draw the entire square on the screen
		{
				// compute the pixel closest to the actual ball center
				C_x = round_double(ball->x);	
				C_y = round_double(ball->y);
				
				draw_centered_square (C_x, C_y, BALL_SIZE, BALL_COLOR);
				ball->showed = 1;
				return;
		}
		
		last_temp_x = ball->last_x;
		last_temp_y = ball->last_y;
		
		// draw a sequence of pixels from last position to the new one
		for(i=1; i<=FLUIDITY_FACTOR; i++)
		{
				t = (double)i / (double)FLUIDITY_FACTOR;

				temp_x = ball->last_x + t*(ball->x - ball->last_x);
				temp_y = ball->last_y + t*(ball->y - ball->last_y);
				
				move_ball_square(round_double(last_temp_x), round_double(last_temp_y), round_double(temp_x), round_double(temp_y));
				
				last_temp_x = temp_x;
				last_temp_y = temp_y;
		}
			
		// re-draw the score if the ball went through the score's text area
		
		if(is_ball_in_player1_score_text_area(ball->last_x, ball->last_y) && 
			!is_ball_in_player1_score_text_area(ball->x, ball->y)) 
		{
				display_score(PLAYER1);
		}
		else if (is_ball_in_player2_score_text_area(ball->last_x, ball->last_y) &&
						!is_ball_in_player2_score_text_area(ball->x, ball->y))	
		{
				display_score(PLAYER2);
		}
		
		// re-draw the paddle if the ball went through one of them
		
		if(is_ball_in_paddle_area(ball->last_x, ball->last_y, &player1_paddle) &&
			!is_ball_in_paddle_area(ball->x,      ball->y,      &player1_paddle))
		{
				player1_paddle.showed = 0;
				draw_paddle(&player1_paddle);
		}
		else if(is_ball_in_paddle_area(ball->last_x, ball->last_y, &player2_paddle) &&
					 !is_ball_in_paddle_area(ball->x,      ball->y,      &player2_paddle))
		{
				player2_paddle.showed = 0;
				draw_paddle(&player2_paddle);
		}
}

// move a square image from the old position (last_C_x, last_C_y) to the new one (C_x, C_y)
void move_ball_square (int last_C_x, int last_C_y, int C_x, int C_y) 
{
		int A_x, A_y, B_x, B_y;
		int distance_x, distance_y;
	
		if(C_x == last_C_x && C_y == last_C_y) return;	// no changes respect to the last image
		
		if(abs_distance(C_x, last_C_x) >= BALL_SIZE || 
			 abs_distance(C_y, last_C_y) >= BALL_SIZE)		// no overlap between last ball image and the new one
		{
				// erase entire old image
				draw_centered_square (last_C_x, last_C_y, BALL_SIZE, BACKGROUND_COLOR);
				// draw entire new image
				draw_centered_square (C_x, C_y, BALL_SIZE, BALL_COLOR);
				return;
		}
		
		// there is overlap -> erase only the right pixels and draw only the new ones
		
		distance_x = C_x - last_C_x;
		distance_y = C_y - last_C_y;
		
		// A -> top-left corner of last image
		A_x = last_C_x - (int)(BALL_SIZE/2);
		A_y = last_C_y - (int)(BALL_SIZE/2);
		// B -> top-left corner of new image
		B_x = C_x - (int)(BALL_SIZE/2);
		B_y = C_y - (int)(BALL_SIZE/2);
			
		if(distance_x >= 0 && distance_y >= 0)		// movement down to the right
		{		
				// erase old parts
				draw_rectangle(A_x, A_y, BALL_SIZE, distance_y, BACKGROUND_COLOR);
				draw_rectangle(A_x, A_y + distance_y, distance_x, BALL_SIZE - distance_y, BACKGROUND_COLOR);
				// draw new parts
				draw_rectangle(B_x + BALL_SIZE - distance_x, B_y, distance_x, BALL_SIZE - distance_y, BALL_COLOR);
				draw_rectangle(B_x, B_y + BALL_SIZE - distance_y, BALL_SIZE, distance_y, BALL_COLOR);
		}
		else if (distance_x <= 0 && distance_y <= 0)	// movement up to the left
		{
				// erase old parts
				draw_rectangle(A_x + BALL_SIZE + distance_x, A_y, -distance_x, BALL_SIZE + distance_y, BACKGROUND_COLOR);
				draw_rectangle(A_x, A_y + BALL_SIZE + distance_y, BALL_SIZE, -distance_y, BACKGROUND_COLOR);
				// draw new parts
				draw_rectangle(B_x, B_y, BALL_SIZE, -distance_y, BALL_COLOR);
				draw_rectangle(B_x, B_y - distance_y, -distance_x, BALL_SIZE + distance_y, BALL_COLOR);
		}
		else if (distance_x > 0 && distance_y < 0) 	// movement up to the right
		{
				// erase old parts
				draw_rectangle(A_x, A_y, distance_x, BALL_SIZE + distance_y, BACKGROUND_COLOR);
				draw_rectangle(A_x, A_y + BALL_SIZE + distance_y, BALL_SIZE, -distance_y, BACKGROUND_COLOR);
				// draw new parts
				draw_rectangle(B_x, B_y, BALL_SIZE, -distance_y, BALL_COLOR);
				draw_rectangle(B_x + BALL_SIZE - distance_x, B_y - distance_y, distance_x, BALL_SIZE + distance_y, BALL_COLOR);
		}
		else if (distance_x < 0 && distance_y > 0)	 // movement down to the left
		{
				// erase old parts
				draw_rectangle(A_x, A_y, BALL_SIZE, distance_y, BACKGROUND_COLOR);
				draw_rectangle(A_x + BALL_SIZE + distance_x, A_y + distance_y, -distance_x, BALL_SIZE - distance_y, BACKGROUND_COLOR);
				// draw new parts
				draw_rectangle(B_x, B_y, -distance_x, BALL_SIZE - distance_y, BALL_COLOR);
				draw_rectangle(B_x, B_y + BALL_SIZE - distance_y, BALL_SIZE, distance_y, BALL_COLOR);
		}
}

// hits controls

// check if right wall is going to be hit by the ball
uint8_t ball_hits_right_wall(double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y;
		
		if(next_x < ball_max_x) return 0;		// no right wall hit
		
		if(x >= ball_max_x) return 0; 		// ball was beyond the right wall x yet
				
		// ball is going beyond the right wall -> compute the touch point
		touch_y = y + ((ball_max_x - x)/(next_x - x))*(next_y - y);
		
		if(touch_y >= wall_min_y && touch_y < wall_max_y)
		{
				*hit_x = ball_max_x;
				*hit_y = touch_y;
				return 1;	// right wall was hit
		}
		
		return 0;		// maybe another element was hit
}

// check if left wall is going to be hit by the ball
uint8_t ball_hits_left_wall(double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y;
		
		if(next_x >= ball_min_x) return 0;		// no left wall hit
		
		if(x < ball_min_x) return 0;	// ball was beyond the left wall x yet
			
		// ball is going beyond the left wall -> compute the touch point
		touch_y = y + ((ball_min_x - x)/(next_x - x))*(next_y - y);
		
		if(touch_y >= wall_min_y && touch_y < wall_max_y)
		{
				*hit_x = ball_min_x;
				*hit_y = touch_y;
				return 1;	// left wall was hit
		}
		
		return 0;		// maybe another element was hit
}

// check if paddle top is going to be hit by the ball
uint8_t ball_hits_paddle_top (Paddle *paddle, double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double ball_max_y = paddle->top_y - (int)(BALL_SIZE/2);
		double touch_x;
		
		if(next_y < ball_max_y) return 0; 	// no paddle top hit
		
		if(y >= ball_max_y) return 0;	// ball was below paddle yet
		
		// ball is going below the top paddle -> compute the touch point
		touch_x = x + ((ball_max_y - y)/(next_y - y))*(next_x - x);
		
		paddle->ball_border_left_x  = (double)(paddle->x) - ((int)(BALL_SIZE/2) + 1);
		paddle->ball_border_right_x = (double)(paddle->x) + PADDLE_WIDTH + ((int)(BALL_SIZE/2) + 1);
		
		if(touch_x >= paddle->ball_border_left_x && touch_x <= paddle->ball_border_right_x)
		{
				*hit_x = touch_x;
				*hit_y = ball_max_y;
				return 1;	// paddle top was hit
		}
		
		return 0;		// maybe another element was hit
}

// check if paddle right is going to be hit by the ball
uint8_t ball_hits_paddle_right (Paddle *paddle, double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y, ball_min_y, ball_max_y;
		double ball_border_right_x = paddle->x + PADDLE_WIDTH + (int)(BALL_SIZE/2);
		
		if(next_x >= ball_border_right_x) return 0; // no paddle right hit
		
		if(x < ball_border_right_x) return 0;	// ball was beyond paddle right yet
		
		// ball is going to pass through the right paddle surface -> compute the touch point
		touch_y = y + ((ball_border_right_x - x)/(next_x - x))*(next_y - y);
			
		ball_min_y = paddle->top_y - (int)(BALL_SIZE/2);
		ball_max_y = paddle->bottom_y + (int)(BALL_SIZE/2);
	
		if(touch_y > ball_min_y && touch_y < ball_max_y) 
		{
				*hit_x = ball_border_right_x;
				*hit_y = touch_y;
				return 1;		// paddle right was hit
		}
		
		return 0; 	// maybe another element was hit
}

// check if paddle left is going to be hit by the ball
uint8_t ball_hits_paddle_left (Paddle *paddle, double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double touch_y, ball_min_y, ball_max_y;
		double ball_border_left_x = paddle->x - ((int)(BALL_SIZE/2) + 1);
		
		if(next_x < ball_border_left_x) return 0; // no paddle left hit
		
		if(x >= ball_border_left_x) return 0;	// ball was beyond paddle left yet
		
		// ball is going to pass through the left paddle surface -> compute the touch point
		touch_y = y + ((ball_border_left_x - x)/(next_x - x))*(next_y - y);
			
		ball_min_y = paddle->top_y - (int)(BALL_SIZE/2);
		ball_max_y = paddle->bottom_y + (int)(BALL_SIZE/2);
	
		if(touch_y > ball_min_y && touch_y < ball_max_y) 
		{
				*hit_x = ball_border_left_x;
				*hit_y = touch_y;
				return 1;		// paddle left was hit
		}
		
		return 0; 	// maybe another element was hit
}

// check if paddle bottom is going to be hit by the ball
uint8_t ball_hits_paddle_bottom (Paddle *paddle, double x, double y, double next_x, double next_y, double *hit_x, double *hit_y)
{
		double ball_min_y = paddle->bottom_y + (int)(BALL_SIZE/2);
		double touch_x;
		
		if(next_y >= ball_min_y) return 0; 	// no paddle bottom hit
		
		if(y < ball_min_y) return 0;	// ball was over the paddle yet
		
		// ball is going over the paddle bottom -> compute the touch point
		touch_x = x + ((ball_min_y - y)/(next_y - y))*(next_x - x);
		
		paddle->ball_border_left_x  = (double)(paddle->x) - ((int)(BALL_SIZE/2) + 1);
		paddle->ball_border_right_x = (double)(paddle->x) + PADDLE_WIDTH + ((int)(BALL_SIZE/2) + 1);
		
		if(touch_x >= paddle->ball_border_left_x && touch_x <= paddle->ball_border_right_x)
		{
				*hit_x = touch_x;
				*hit_y = ball_min_y;
				return 1;	// paddle bottom was hit
		}
		
		return 0;		// maybe another element was hit
}


// check if ball fell under the screen
uint8_t ball_fell_down (double y)
{
		return y >= MAX_Y + BALL_SIZE;
}

// check if ball fell over the screen
uint8_t ball_fell_up (double y)
{
		return y <= -BALL_SIZE;
}

// ball bouncing functions below

void ball_bounces_horizontally (double *x, double *y, double *next_x, double reflection_x, double reflection_y, double *next_v_x, double *v_angle)
{
		double x_to_reflect = *next_x;
		double v_x_to_reflect = *next_v_x;
	
		// update origin coordinates
		*x = reflection_x;
		*y = reflection_y;
		
		// apply reflection respect to vertical axis
		*next_x = 2*reflection_x - x_to_reflect;		// new x-coordinate
		*next_v_x = -v_x_to_reflect;								// new v_x
		*v_angle = (PI - *v_angle) > 0 ? (PI - *v_angle) : (3.0 * PI - *v_angle);
}

void ball_bounces_vertically (double *x, double *y, double *next_y, double reflection_x, double reflection_y, double *next_v_y, double *v_angle)
{
		double y_to_reflect = *next_y;
		double v_y_to_reflect = *next_v_y;
	
		// update origin coordinates
		*x = reflection_x;
		*y = reflection_y;
		
		// apply reflection respect to horizontal axis
		*next_y = 2*reflection_y - y_to_reflect;		// new y-coordinate
		*next_v_y = -v_y_to_reflect;								// new v_y
		*v_angle = 2.0*PI - *v_angle; 							// new velocity angle
}

void ball_bounces_on_paddle (Paddle *paddle, double *x, double *y, double *next_x, double *next_y, double reflection_x, 
														  double reflection_y, double *next_v_x, double *next_v_y, double *v_angle)
{
		double old_v_angle, new_v_angle, rotation_angle, paddle_extended_width, angles_range;
	
		// update origin coordinates
		*x = reflection_x;
		*y = reflection_y;
		
		// compute these 2 terms in order to make the new angle formula simpler
		paddle_extended_width = paddle->ball_border_right_x - paddle->ball_border_left_x;
		angles_range = paddle->reflection_angle_2 - paddle->reflection_angle_1;
	
		// compute new velocity angle
		// the new angle depends linearly on the paddle point touched by the ball
		new_v_angle = paddle->reflection_angle_1 + angles_range*(reflection_x - paddle->ball_border_left_x)/paddle_extended_width;
		
		// compute the new velocity coordinates (v_x, v_y) maintaining the same velocity module (BALL_V)
		*next_v_x = BALL_V * cos(new_v_angle);
		*next_v_y = BALL_V * sin(new_v_angle);
	
		// computing rotation angle 
		old_v_angle = *v_angle;
		rotation_angle = new_v_angle - old_v_angle;
		
		// computing new (x,y) coordinates applying a rotation over the paddle touched point
		*next_x = (*next_x - reflection_x)*cos(rotation_angle) - (*next_y - reflection_y)*sin(rotation_angle) + reflection_x;
		*next_y = (*next_x - reflection_x)*sin(rotation_angle) + (*next_y - reflection_y)*cos(rotation_angle) + reflection_y;
		
		// update v angle
		*v_angle = new_v_angle;
}

// read & set flags

void move (Ball *ball, uint8_t moving)
{
		ball->moving = moving;
}

uint8_t has_to_be_moved (Ball *ball)
{
		return ball->moving;
}

void show_ball (Ball *ball, uint8_t show)
{
		ball->showed = show;
}

