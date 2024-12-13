#include "RIT.h"


void joystick_init(void) {

    // Set joystick DOWN (pin 1.26)
    LPC_PINCON->PINSEL3 &= ~(3<<20); // SET pin 20, 21 to 00 in PINSEL3
    LPC_GPIO1->FIODIR   &= ~(1<<26); // P1.26 Input

    // Set joystick LEFT (pin 1.27)
    LPC_PINCON->PINSEL3 &= ~(3<<22); // SET pin 22, 23 to 00 in PINSEL3
    LPC_GPIO1->FIODIR   &= ~(1<<27);

    // Set joystick RIGHT (pin 1.28)
    LPC_PINCON->PINSEL3 &= ~(3<<24); // SET pin 24, 25 to 00 in PINSEL3
    LPC_GPIO1->FIODIR   &= ~(1<<28);

    // Set joystick UP (pin 1.29)
    LPC_PINCON->PINSEL3 &= ~(3<<26); // SET pin 26, 27 to 00 in PINSEL3
    LPC_GPIO1->FIODIR   &= ~(1<<29); // P1.29 Input (joystick on PORT1 defined as Input)

}