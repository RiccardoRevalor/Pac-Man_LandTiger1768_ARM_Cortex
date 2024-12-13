/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include <stdio.h> /*for sprintf*/

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
	switch (playerDir) {
    case RIGHT_DIR: {
        int nextX = plX + 1; // Spostamento a destra
        if (maze[plY][nextX] == FREE_CODE) {
            // Il giocatore può muoversi
            erasePlayer(plX, plY);
            plX = nextX;
            drawPlayer(plX, plY, playerDir);
        } else if (maze[plY][nextX] == STDPILL_CODE_1 || maze[plY][nextX] == STDPILL_CODE_2) {
            // Mangia la STDPILL
            // TODO: gestione punteggio o stato
            erasePlayer(plX, plY);
            plX = nextX;
            drawPlayer(plX, plY, playerDir);
        }
        break;
    }
    case LEFT_DIR: {
        int nextX = plX - 1; // Spostamento a sinistra
        if (maze[plY][nextX] == FREE_CODE) {
            erasePlayer(plX, plY);
            plX = nextX;
            drawPlayer(plX, plY, playerDir);
        } else if (maze[plY][nextX] == STDPILL_CODE_1 || maze[plY][nextX] == STDPILL_CODE_2) {
            erasePlayer(plX, plY);
            plX = nextX;
            drawPlayer(plX, plY, playerDir);
        }
        break;
    }
    case UP_DIR: {
        int nextY = plY - 1; // Spostamento in alto
        if (maze[nextY][plX] == FREE_CODE) {
            erasePlayer(plX, plY);
            plY = nextY;
            drawPlayer(plX, plY, playerDir);
        } else if (maze[nextY][plX] == STDPILL_CODE_1 || maze[nextY][plX] == STDPILL_CODE_2) {
            erasePlayer(plX, plY);
            plY = nextY;
            drawPlayer(plX, plY, playerDir);
        }
        break;
    }
    case DOWN_DIR: {
        int nextY = plY + 1; // Spostamento in basso
        if (maze[nextY][plX] == FREE_CODE) {
            erasePlayer(plX, plY);
            plY = nextY;
            drawPlayer(plX, plY, playerDir);
        } else if (maze[nextY][plX] == STDPILL_CODE_1 || maze[nextY][plX] == STDPILL_CODE_2) {
            erasePlayer(plX, plY);
            plY = nextY;
            drawPlayer(plX, plY, playerDir);
        }
        break;
    }
}

  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
