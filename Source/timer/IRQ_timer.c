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
#include "../../game.h"

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
	
	static uint8_t lastDir = RIGHT_DIR;
	static uint8_t isIdle = 0;
	char buf[100];
	
	switch (playerDir) {
    case RIGHT_DIR: {
				isIdle = 0;
			//plX is the left upper cell of the player
				int lookAheadX = plX + 2; 
        int nextX = plX + 1; 
        if (maze[plY][lookAheadX] == FREE_CODE && maze[plY+1][lookAheadX] == FREE_CODE) {
            // Il giocatore può muoversiS
            erasePlayer(plX, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
						//drawBlank(nextX - 1, plY);
        } else if ((maze[plY][lookAheadX] == STDPILL_CODE_1 || maze[plY][lookAheadX] == STDPILL_CODE_2) && (maze[plY+1][lookAheadX] == STDPILL_CODE_1 || maze[plY+1][lookAheadX] == STDPILL_CODE_2)) {
            // Mangia la STDPILL
            // TODO: gestione punteggio o stato
            erasePlayer(plX, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
        } else if (((maze[plY][lookAheadX] == STDPILL_CODE_1 || maze[plY][lookAheadX] == STDPILL_CODE_2) && maze[plY+1][lookAheadX] == FREE_CODE) || ((maze[plY+1][lookAheadX] == STDPILL_CODE_1 || maze[plY+1][lookAheadX] == STDPILL_CODE_2) && maze[plY][lookAheadX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
				
					//2 cases: upper lookahead block contains STDPILL_CODE_2 or lower lookahead block contains STDPILL_CODE_2
					if (maze[plY][lookAheadX] == STDPILL_CODE_1 || maze[plY][lookAheadX] == STDPILL_CODE_2) {
						//get the "source cell" (so the leftmost upper of the 2x2 square) cell of the pill
						//and delete the FULL pill
						erasePill(lookAheadX, plY-1);
					} else {
						//this is just the leftmost upper cell of the 2x2 pill
						erasePill(lookAheadX, plY);
					}
					erasePlayer(plX, plY);
          drawPlayer(nextX, plY, playerDir, 1);
					plX = nextX;
				
				}else {
					playerDir = IDLE_DIR;
				}
				//drawBlank(nextX, plY);
				
				//DEBUG MOVEMENTS
				if (DEBUG_MOVS == 1) {
					drawBlank(lookAheadX + 1, plY);
					drawBlank(lookAheadX + 1, plY + 1);
					playerDir = IDLE_DIR;
					sprintf(buf, "X0:%d Y0:%d, X1:%d Y1:%d", lookAheadX + 1, plY, lookAheadX + 1, plY+1);
					GUI_Text(10, 300, (uint8_t *) buf, Red, White);
					
				}
        break;
    }
    case LEFT_DIR: {
				isIdle = 0;
			  //in this case the player matrix is first rotated
				//so the lookAheadX is equivalent to nextX since we always consider the upper left cell
        int nextX = plX - 1; // Spostamento a sinistra
        if (maze[plY][nextX] == FREE_CODE && maze[plY+1][nextX] == FREE_CODE) {
						//PURE CHUNK OF 2x2 FREE BLOCKS
            erasePlayer(plX, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
        } else if ((maze[plY][nextX] == STDPILL_CODE_1 || maze[plY][nextX] == STDPILL_CODE_2) && (maze[plY+1][nextX] == STDPILL_CODE_1 || maze[plY+1][nextX] == STDPILL_CODE_2)) {
						//PURE CHUNK OF 2x2 STDPILL BLOCKS
            // Mangia la STDPILL
            // TODO: gestione punteggio o stato
            erasePlayer(plX, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
        } else if (((maze[plY][nextX] == STDPILL_CODE_1 || maze[plY][nextX] == STDPILL_CODE_2) && maze[plY+1][nextX] == FREE_CODE) || ((maze[plY+1][nextX] == STDPILL_CODE_1 || maze[plY+1][nextX] == STDPILL_CODE_2) && maze[plY][nextX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
					erasePlayer(plX, plY);
          drawPlayer(nextX, plY, playerDir, 1);
					plX = nextX;
				
				}else {
					playerDir = IDLE_DIR;
				}
				
				//DEBUG MOVEMENTS
				if (DEBUG_MOVS == 1) {
					drawBlank(nextX - 1, plY);
					drawBlank(nextX - 1, plY + 1);
					playerDir = IDLE_DIR;
					sprintf(buf, "X0:%d Y0:%d, X1:%d Y1:%d", nextX - 1, plY, nextX - 1, plY+1);
					GUI_Text(10, 300, (uint8_t *) buf, Red, White);
				}
        break;
    }
    case UP_DIR: {
				isIdle = 0;
				//in this case the player matrix is first rotated
				//so lookAheadY is equivalent to nextY
        int nextY = plY - 1; // Spostamento in alto
        if (maze[nextY][plX] == FREE_CODE && maze[nextY][plX+1] == FREE_CODE) {
            erasePlayer(plX, plY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if ((maze[nextY][plX] == STDPILL_CODE_1 || maze[nextY][plX] == STDPILL_CODE_2) && (maze[nextY][plX+1] == STDPILL_CODE_1 || maze[nextY][plX+1] == STDPILL_CODE_2)) {
            erasePlayer(plX, plY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[nextY][plX] == STDPILL_CODE_1 || maze[nextY][plX] == STDPILL_CODE_2) && maze[nextY][plX+1] == FREE_CODE) || ((maze[nextY][plX+1] == STDPILL_CODE_1 || maze[nextY][plX+1] == STDPILL_CODE_2) && maze[nextY][plX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
						erasePlayer(plX, plY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
				
				} else {
					playerDir = IDLE_DIR;
					
				}
				
				if (DEBUG_MOVS == 1) {
					drawBlank(plX, nextY-1);
					drawBlank(plX+1, nextY-1);
					playerDir = IDLE_DIR;
					sprintf(buf, "X0:%d Y0:%d, X1:%d Y1:%d", plX, nextY-1, plX+1,nextY-1);
					GUI_Text(10, 300, (uint8_t *) buf, Red, White);
				}
        break;
    }
    case DOWN_DIR: {
				isIdle = 0;
				//player matrix is first rotated
				//here we have lookAheadY different from nextY
				int lookAheadY = plY + 2;
        int nextY = plY + 1; // Spostamento in basso
        if (maze[lookAheadY][plX] == FREE_CODE && maze[lookAheadY][plX+1] == FREE_CODE) {
            erasePlayer(plX, plY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if ((maze[lookAheadY][plX] == STDPILL_CODE_1 || maze[lookAheadY][plX] == STDPILL_CODE_2) && (maze[lookAheadY][plX+1] == STDPILL_CODE_1 || maze[lookAheadY][plX+1] == STDPILL_CODE_2)) {
            erasePlayer(plX, plY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[lookAheadY][plX] == STDPILL_CODE_1 || maze[lookAheadY][plX] == STDPILL_CODE_2) && maze[lookAheadY][plX+1] == FREE_CODE) || ((maze[lookAheadY][plX+1] == STDPILL_CODE_1 || maze[lookAheadY][plX+1] == STDPILL_CODE_2) && maze[lookAheadY][plX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
						erasePlayer(plX, plY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
				
				} else {
					playerDir = IDLE_DIR;
					//LCD_Clear(Blue);
				}
				
				
				if (DEBUG_MOVS == 1) {
					drawBlank(plX, lookAheadY+1);
					drawBlank(plX+1, lookAheadY+1);
					playerDir = IDLE_DIR;
					sprintf(buf, "X0:%d Y0:%d, X1:%d Y1:%d", plX, lookAheadY+1, plX+1,lookAheadY+1);
					GUI_Text(10, 300, (uint8_t *) buf, Red, White);
				}
        break;
    }
		
		case IDLE_DIR: {
			//in case the player stops, draw the animation of teh last dir (and not the playerEating sprite)
			
			if (isIdle == 0) {
				erasePlayer(plX, plY);
				drawPlayer(plX, plY, lastDir, 0);
				isIdle = 1;
			}
		}
		
		lastDir = playerDir;
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
