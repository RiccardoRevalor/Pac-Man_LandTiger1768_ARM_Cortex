#include "button.h"
#include "LPC17xx.h"

void BUTTON_init(void) {
	
	//Initialize button EINT0 -> pin 2.10
  LPC_PINCON->PINSEL4    |= (1 << 20);		 /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 10);    /* PORT2.10 defined as input          */
		 
  NVIC_EnableIRQ(EINT0_IRQn);              /* enable irq in nvic                 */
	NVIC_SetPriority(EINT0_IRQn, 0);				 
}
