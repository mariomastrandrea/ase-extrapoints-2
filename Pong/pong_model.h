#ifndef PONG_MODEL_DEF
#define PONG_MODEL_DEF

#include <stdint.h>

#define PLAYER1 1
#define PLAYER2 2

#define WIN_SCORE 5

void initialize_game_field (void);
void reset_game (void);		// (INT0 button)
void start_game (void);		// (KEY1 button)
void wait_and_restart (void);	// restart game after new score
void pause (void);				// (KEY2 button)
void clear_pause(void);

uint8_t increase_score (uint8_t player);		// increase and display new score
void display_new_score (uint8_t player, uint32_t last_score, uint32_t new_score);
void display_score (uint8_t player);

void won (uint8_t player);
void clear_won (uint8_t player);

#endif
