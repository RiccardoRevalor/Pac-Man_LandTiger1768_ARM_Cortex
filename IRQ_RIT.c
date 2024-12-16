#include "LPC17xx.h"
#include "RIT.h"
#include "button.h"

volatile int EINT0_down = 0;
void RIT_IRQHandler(void) {
	
	
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
	
	
	//BUTTON EINT0 Management and debouncing
	if (EINT0_down >= 1) {
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	//EINT0 PRESSED
			EINT0_down++;
		} else {
			//button released
			EINT0_down = 0; //reset counter
			//enable interrupt mode again for EINT0 button
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4  |= (1 << 20);     /* External interrupt 0 pin selection */
			//LCD_Clear(Blue);
			//show pause
		}
	} else {
    // Se `EINT0_down` è 0 e il bottone è in modalità GPIO, ripristina a EINT0
    if ((LPC_PINCON->PINSEL4 & (3 << 20)) == 0) { // Controlla se è in modalità GPIO
        LPC_PINCON->PINSEL4 |= (1 << 20); // Ripristina a EINT0
        NVIC_EnableIRQ(EINT0_IRQn);
    }
	}
	 
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	 
	 return;
}