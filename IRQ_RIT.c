#include "RIT.h"

extern uint8_t playerDir; 

void RIT_IRQHandler(void) {
	
	
	//JOYSTICK UP PRESSED -> SET DIR TO UP -> 2
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		playerDir = UP_DIR;
	}
	
	//JOYSTICK DOWN PRESSED -> SET DIR TO DOWN -> 3
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		playerDir = DOWN_DIR;
	}
	
	if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		playerDir = LEFT_DIR;
	}
	
	 if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		 playerDir = RIGHT_DIR;
	 }
	 
	 
	 LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	 
	 return;
}