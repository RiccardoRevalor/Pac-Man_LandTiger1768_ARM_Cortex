#ifndef __RIT_H
#define __RIT_H

#include "game.h"
#include "LPC17xx.h"

/* lib_RIT.c */
extern uint32_t init_RIT( uint32_t RITInterval );
extern void enable_RIT( void );
extern void disable_RIT( void );
extern void reset_RIT( void );
/* IRQ_RIT.c */
extern void RIT_IRQHandler (void);
/* joystick.c */
void joystick_init(void);

#endif /* end __RIT_H */