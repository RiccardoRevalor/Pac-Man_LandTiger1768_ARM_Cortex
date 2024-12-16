#ifndef __BUTTON_H
#define __BUTTON_H

#include "LPC17xx.h"

void BUTTON_init(void);

void EINT1_IRQHandler(void);

#endif