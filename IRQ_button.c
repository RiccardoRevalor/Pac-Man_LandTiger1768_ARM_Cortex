#include "button.h"
#include "LPC17xx.h"

extern int EINT0_down;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	
	/*
	RIT TIMER IS COUNTING
	AS SOON I PRESS THIS BTN THE INTERRUPT IS DISABLED, DOWN = 1 DOWN IS EXTERN VARIABLE
	*/
	
	//DISable future interrupts of this button
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	//I have to set the button temporarily to gpio
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	EINT0_down=1;
	
	
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}
