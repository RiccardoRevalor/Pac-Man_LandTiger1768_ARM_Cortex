#include "LPC17xx.h"
#include "RIT.h"
#include "button.h"

volatile int EINT0_down = 0;
void RIT_IRQHandler(void) {
	
	if (isPaused == 0) {
		//GUI_Text(10, 10, (uint8_t *) "RIT Active", White, BACKGROUND_COLOR);
		//JOYSTICK UP PRESSED -> SET DIR TO UP -> 2
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
			playerDir = UP_DIR;
		}
		
		//JOYSTICK DOWN PRESSED -> SET DIR TO DOWN -> 3
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
			playerDir = DOWN_DIR;
			//LCD_Clear(Blue);
		}
		
		if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
			playerDir = LEFT_DIR;
		}
		
		if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
			playerDir = RIGHT_DIR;
		}
	}
	//GUI_Text(10, 40, (uint8_t *) "RIT Active2", White, BACKGROUND_COLOR);
	
	
	//BUTTON EINT0 Management and debouncing
	if (EINT0_down >= 1) {
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	//EINT0 PRESSED
			EINT0_down++;
		} else {
			//enable interrupt mode again for EINT0 button
			//LCD_Clear(Blue);
			
			//CASES
			//1) game paused and firstGame == 1 -> start a new game
			//2) game paused and firstGame == 0 -> resume existing game
			
			if (firstGame == 1) {
				//start a new game from the beginning
				//set isPaused to 0
				//set firstGame to 0
				isPaused = 0;
				firstGame = 0;
				newGameRoutine();
				NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
				LPC_PINCON->PINSEL4  |= (1 << 20);     /* External interrupt 0 pin selection */
				EINT0_down = 0;
			} else {
				//an existing game has been already running
				if (isPaused == 0) {
					//put the existing game in pause state
					//set isPaused to 1
					isPaused = 1;
					showPause(1);					
					NVIC_EnableIRQ(EINT0_IRQn);							 
					LPC_PINCON->PINSEL4  |= (1 << 20);     
					EINT0_down = 0;
				} else {
					//un-pause the game, so resume it
					//se isPaused to 0
					isPaused = 0;
					LCD_Clear(BACKGROUND_COLOR);
					resumeGameRoutine();
					NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
					LPC_PINCON->PINSEL4  |= (1 << 20);     /* External interrupt 0 pin selection */
					EINT0_down = 0;
				}
			}
			
			
			
			
			//Reactivate EINT0 ONLY at the END!!
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4  |= (1 << 20);     /* External interrupt 0 pin selection */
			
			
			//button released
			EINT0_down = 0; //reset counter
			
		}
	} /*else {
    // Se `EINT0_down` è 0 e il bottone è in modalità GPIO, ripristina a EINT0
    if ((LPC_PINCON->PINSEL4 & (3 << 20)) == 0) { // Controlla se è in modalità GPIO
        LPC_PINCON->PINSEL4 |= (1 << 20); // Ripristina a EINT0
        NVIC_EnableIRQ(EINT0_IRQn);
    }
	}*/
	 
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	 
	 return;
}