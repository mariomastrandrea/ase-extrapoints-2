// Includes 
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "button_EXINT/button.h"
#include "adc/adc.h"
#include "dac/dac.h"
#include "Pong/pong_model.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int main(void)
{
		uint32_t Timer0_MR0, Timer1_MR0, Timer3_MR0;
		uint32_t _50ms = 0x1312D0;	// 50ms * 25MHz =  1 250 000 = 0x1312D0
	
		// initializations
		SystemInit();  									/* System Initialization (i.e., PLL)  */
	
		// buttons
		BUTTON_init();
	
		// ADC & DAC - Potentiometer & Speaker
		ADC_init();
		DAC_init();
	
		// LCD 
		LCD_Initialization();
			
		// Timers
		
		// 	-	TIMER0 -> (50ms) to control bouncing and de-bouncing; active only after pressing a button
		Timer0_MR0 = _50ms;				
		init_timer(0, Timer0_MR0, 0);				
			
		//  - TIMER1 -> (50ms) to control potentiometer & ball movements; always active during game
		Timer1_MR0 = _50ms; 
		init_timer(1, Timer1_MR0, 0);			
		enable_timer(1);
		
		// - TIMER2 -> to play sounds; active only when playing sound
		
		// - TIMER3 -> (0.5s) to restart game after a new score
		Timer3_MR0 = _50ms * 6;	// 500ms = 0.5s
		init_timer(3, Timer3_MR0, 0);
		
		/* START THE PONG GAME HERE */
		initialize_game_field();
		reset_game();							
		
		LPC_SC->PCON |= 0x1;									/* power-down	mode	*/
		LPC_SC->PCON &= ~(0x2);						
		
		while (1)	{
				__ASM("wfi");
		}
}

