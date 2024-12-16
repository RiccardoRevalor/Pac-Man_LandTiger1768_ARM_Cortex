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
extern uint16_t score;
extern uint16_t life;
extern uint16_t gameTime;
char scoreS[5];
char gameTimeS[3];
static uint8_t scoreNeedsRedraw = 0;
static uint8_t lifeNeedsRedraw = 0;

void updateScoreString(){
	if (scoreNeedsRedraw == 1){
		//clear the number part of the score string
		GUI_Text(getPixelX(SCORE_X+10), getPixelY(SCORE_Y), (uint8_t *) "    ", BACKGROUND_COLOR, BACKGROUND_COLOR);
		scoreNeedsRedraw = 0;
	}
	//create a new string 
	sprintf(scoreS, "%04d", score);
	GUI_Text(getPixelX(SCORE_X+10), getPixelY(SCORE_Y), (uint8_t *) scoreS, White, BACKGROUND_COLOR);
}

void updateGameTimeString(){
	sprintf(gameTimeS, "%02d", gameTime);
	GUI_Text(getPixelX(TIMECNT_X+8), getPixelY(TIMECNT_Y), (uint8_t *) gameTimeS, White, BACKGROUND_COLOR);
	
}



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
				
						//Score update
						score+=10;
						scoreNeedsRedraw=1;
						//updateScoreString();
				
            erasePlayer(plX, plY);
						erasePill(lookAheadX, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
			
						
        } else if (((maze[plY][lookAheadX] == STDPILL_CODE_1 || maze[plY][lookAheadX] == STDPILL_CODE_2) && maze[plY+1][lookAheadX] == FREE_CODE) || ((maze[plY+1][lookAheadX] == STDPILL_CODE_1 || maze[plY+1][lookAheadX] == STDPILL_CODE_2) && maze[plY][lookAheadX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
					
					//Score update
					score+=10;
					scoreNeedsRedraw=1;
				
				
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
				
				} else if ((maze[plY][lookAheadX] == PWRPILL_CODE_1 || maze[plY][lookAheadX] == PWRPILL_CODE_2) && (maze[plY+1][lookAheadX] == PWRPILL_CODE_1 || maze[plY+1][lookAheadX] == PWRPILL_CODE_2)) {
            // Mangia la PWRPILL
            // TODO: gestione punteggio o stato
				
						//Score update
						score+=50;
						scoreNeedsRedraw=1;
						//updateScoreString();
				
            erasePlayer(plX, plY);
						erasePill(lookAheadX, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
			
						
        } else if (((maze[plY][lookAheadX] == PWRPILL_CODE_1 || maze[plY][lookAheadX] == PWRPILL_CODE_2) && maze[plY+1][lookAheadX] == FREE_CODE) || ((maze[plY+1][lookAheadX] == PWRPILL_CODE_1 || maze[plY+1][lookAheadX] == PWRPILL_CODE_2) && maze[plY][lookAheadX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
					
					//Score update
					score+=50;
					scoreNeedsRedraw=1;
				
				
					//2 cases: upper lookahead block contains STDPILL_CODE_2 or lower lookahead block contains STDPILL_CODE_2
					if (maze[plY][lookAheadX] == PWRPILL_CODE_1 || maze[plY][lookAheadX] == PWRPILL_CODE_2) {
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
				
				} else if (maze[plY][lookAheadX] == TUNNEL_CODE && maze[plY+1][lookAheadX] == TUNNEL_CODE){
					//the next player position is the entrance of the LEFT TUNNEL
					//the player will be teleported to the RIGTH TUNNEL mantaining the same direction
					//set the new player's coordinates!
					erasePlayer(plX, plY);
					drawPlayer(LT_X+1, plY, playerDir, 1);
					plX = LT_X+1;
				} else {
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
				
						//Score update
						score+=10;
						scoreNeedsRedraw=1;
				
            erasePlayer(plX, plY);
						erasePill(nextX-1, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
        } else if (((maze[plY][nextX] == STDPILL_CODE_1 || maze[plY][nextX] == STDPILL_CODE_2) && maze[plY+1][nextX] == FREE_CODE) || ((maze[plY+1][nextX] == STDPILL_CODE_1 || maze[plY+1][nextX] == STDPILL_CODE_2) && maze[plY][nextX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
				
					//Score update
					score+=10;
					scoreNeedsRedraw=1;
				
				
					if (maze[plY][nextX] == STDPILL_CODE_1 || maze[plY][nextX] == STDPILL_CODE_2) {
						//find the leftmost upper cell of the pill and erase the whole pill
						erasePill(nextX-1, plY-1);
					} else {
						erasePill(nextX-1, plY);
					}
					erasePlayer(plX, plY);
          drawPlayer(nextX, plY, playerDir, 1);
					plX = nextX;
				
				} else if ((maze[plY][nextX] == PWRPILL_CODE_1 || maze[plY][nextX] == PWRPILL_CODE_2) && (maze[plY+1][nextX] == PWRPILL_CODE_1 || maze[plY+1][nextX] == PWRPILL_CODE_2)) {
						//PURE CHUNK OF 2x2 PWRPILL BLOCKS
            // Mangia la STDPILL
            // TODO: gestione punteggio o stato
				
						//Score update
						score+=50;
						scoreNeedsRedraw=1;
				
            erasePlayer(plX, plY);
						erasePill(nextX-1, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
        } else if (((maze[plY][nextX] == PWRPILL_CODE_1 || maze[plY][nextX] == PWRPILL_CODE_2) && maze[plY+1][nextX] == FREE_CODE) || ((maze[plY+1][nextX] == PWRPILL_CODE_1 || maze[plY+1][nextX] == PWRPILL_CODE_2) && maze[plY][nextX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
				
					//Score update
					score+=50;
					scoreNeedsRedraw=1;
				
				
					if (maze[plY][nextX] == PWRPILL_CODE_1 || maze[plY][nextX] == PWRPILL_CODE_2) {
						//find the leftmost upper cell of the pill and erase the whole pill
						erasePill(nextX-1, plY-1);
					} else {
						erasePill(nextX-1, plY);
					}
					erasePlayer(plX, plY);
          drawPlayer(nextX, plY, playerDir, 1);
					plX = nextX;
				
				} else if (maze[plY][nextX] == TUNNEL_CODE && maze[plY+1][nextX] == TUNNEL_CODE){
					//the next player position is the entrance of the LEFT TUNNEL
					//the player will be teleported to the RIGTH TUNNEL mantaining the same direction
					//set the new player's coordinates!
					erasePlayer(plX, plY);
					drawPlayer(RT_X-2, plY, playerDir, 1);
					plX = RT_X-2;
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
            
						//Score update
						score+=10;
						scoreNeedsRedraw=1;
				
				
						erasePlayer(plX, plY);
						erasePill(plX, nextY-1);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[nextY][plX] == STDPILL_CODE_1 || maze[nextY][plX] == STDPILL_CODE_2) && maze[nextY][plX+1] == FREE_CODE) || ((maze[nextY][plX+1] == STDPILL_CODE_1 || maze[nextY][plX+1] == STDPILL_CODE_2) && maze[nextY][plX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
						
				
						//Score update
						score+=10;
						scoreNeedsRedraw=1;
				
						if (maze[nextY][plX] == STDPILL_CODE_1 || maze[nextY][plX] == STDPILL_CODE_2){
							erasePill(plX-1, nextY-1);
						} else {
							erasePill(plX+1, nextY-1);
						}
						erasePlayer(plX, plY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
				
				} else if ((maze[nextY][plX] == PWRPILL_CODE_1 || maze[nextY][plX] == PWRPILL_CODE_2) && (maze[nextY][plX+1] == PWRPILL_CODE_1 || maze[nextY][plX+1] == PWRPILL_CODE_2)) {
            //FULL PWR PILL 
						//Score update
						score+=50;
						scoreNeedsRedraw=1;
				
				
						erasePlayer(plX, plY);
						erasePill(plX, nextY-1);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[nextY][plX] == PWRPILL_CODE_1 || maze[nextY][plX] == PWRPILL_CODE_2) && maze[nextY][plX+1] == FREE_CODE) || ((maze[nextY][plX+1] == PWRPILL_CODE_1 || maze[nextY][plX+1] == PWRPILL_CODE_2) && maze[nextY][plX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
						
				
						//Score update
						score+=50;
						scoreNeedsRedraw=1;
				
						if (maze[nextY][plX] == PWRPILL_CODE_1 || maze[nextY][plX] == PWRPILL_CODE_2){
							erasePill(plX-1, nextY-1);
						} else {
							erasePill(plX+1, nextY-1);
						}
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
            
						//Score update
						score+=10;
						scoreNeedsRedraw=1;
				
				
						erasePlayer(plX, plY);
						erasePill(plX, lookAheadY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[lookAheadY][plX] == STDPILL_CODE_1 || maze[lookAheadY][plX] == STDPILL_CODE_2) && maze[lookAheadY][plX+1] == FREE_CODE) || ((maze[lookAheadY][plX+1] == STDPILL_CODE_1 || maze[lookAheadY][plX+1] == STDPILL_CODE_2) && maze[lookAheadY][plX] == FREE_CODE)) {
						//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
						
						//Score update
						score+=10;
						scoreNeedsRedraw=1;
				
				
						if (maze[lookAheadY][plX] == STDPILL_CODE_1 || maze[lookAheadY][plX] == STDPILL_CODE_2) {
							erasePill(plX-1, lookAheadY);
						} else {
							erasePill(plX+1, lookAheadY);
						}
						erasePlayer(plX, plY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
				
				} else if ((maze[lookAheadY][plX] == PWRPILL_CODE_1 || maze[lookAheadY][plX] == PWRPILL_CODE_2) && (maze[lookAheadY][plX+1] == PWRPILL_CODE_1 || maze[lookAheadY][plX+1] == PWRPILL_CODE_2)) {
            //FULL PWR PILL
						//Score update
						score+=50;
						scoreNeedsRedraw=1;
				
				
						erasePlayer(plX, plY);
						erasePill(plX, lookAheadY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[lookAheadY][plX] == PWRPILL_CODE_1 || maze[lookAheadY][plX] == PWRPILL_CODE_2) && maze[lookAheadY][plX+1] == FREE_CODE) || ((maze[lookAheadY][plX+1] == PWRPILL_CODE_1 || maze[lookAheadY][plX+1] == PWRPILL_CODE_2) && maze[lookAheadY][plX] == FREE_CODE)) {
						//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
						
						//Score update
						score+=50;
						scoreNeedsRedraw=1;
				
				
						if (maze[lookAheadY][plX] == PWRPILL_CODE_1 || maze[lookAheadY][plX] == PWRPILL_CODE_2) {
							erasePill(plX-1, lookAheadY);
						} else {
							erasePill(plX+1, lookAheadY);
						}
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
				
				if (scoreNeedsRedraw==1){
					updateScoreString();
				}
				
			}
		}
		
		lastDir = playerDir;
}
	

	/*
	//Update GUI TEXT
	//Write 'SCORE' string
	//MAX achievable score: 236 * 10 + 6*50 = 2660 -> 4 digits
	GUI_Text(getPixelX(SCORE_X), getPixelY(SCORE_Y), (uint8_t *) "SCORE:0000", White, BACKGROUND_COLOR);
	//60 seconds time counter
	GUI_Text(getPixelX(TIMECNT_X), getPixelY(TIMECNT_Y), (uint8_t *) "60", White, BACKGROUND_COLOR);
	//Lives
	char lifeS[100];
	sprintf(lifeS, "Lives: %d", life);
	GUI_Text(getPixelX(LIFECNT_X), getPixelY(LIFECNT_Y), (uint8_t *) lifeS, White, BACKGROUND_COLOR);
*/

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
	

	/*
	
	TIMER 1
	Tasks:
	1) every n seconds, generate a power pill until we have a total of 6 powerpills (conuting even the already eaten ones) on the map
	2) every second decrement the 60 seconds time counter
	3) if the timer counter reaches 0, show GAME OVER Screen
	
	*/
	
	
	//every 10 seconds, add a new power pill
	//so after 10 seconds -> 1 power pill
	//after 20 seconds -> 2 power pill (counting also the-maybe-eaten ones)
	//and so on...
	if (gameTime % 10 == 0){
		if (pwrPillsCounter < 6) {
			
			//generate a new power pill
			//choose the line in which turn a std pill into a power pill
			int totalPills = 0;
			int x, y;

			// Conta il numero totale di pillole nella matrice
			for (y = MAZESTART + 1; y < YMAX - MAZESTART - 1; y++) {
					for (x = 1; x < XMAX - 1; x++) {
							if (maze[y][x] == STDPILL_CODE_1) {
									totalPills++;
							}
					}
			}
			
			if (totalPills > 0) {
				int targetPill = rand() % totalPills + 1; // Seleziona l'ennesima pillola
				int currentPill = 0;

				for (y = MAZESTART + 1; y < YMAX - MAZESTART - 1; y++) {
						for (x = 1; x < XMAX - 1; x++) {
								if (maze[y][x] == STDPILL_CODE_1) {
										currentPill++;
										if (currentPill == targetPill) {
												// Trasforma questa pillola in una power pill
												maze[y][x] = PWRPILL_CODE_1; // Top Left -> set the pwrpill code
												//set the other 3 cells (at positions (x+1,y),(x+1,y+1),(x,y+1)) to PWRPILL_CODE_2
												maze[y][x + 1] = PWRPILL_CODE_2;   // Top-right
												maze[y + 1][x] = PWRPILL_CODE_2;   // Bottom-left
												maze[y + 1][x + 1] = PWRPILL_CODE_2; // Bottom-right
												//draw the PWRPILL setting 1 to the power pill boolean case
												drawPills4(x, y, 1);
											
												//increment the power pills counter by 1 unit
												++pwrPillsCounter;
										}
								}
						}
				}
				
				
				
			} else {
				//VICTORY!
				LCD_Clear(Green);
				
			}
		}
		
	} 
	
	//decrement gameTime by 1 time unit
	--gameTime;
	updateGameTimeString();
	
	if (gameTime <= 0) {
		
		//RESET AND DISABLE ALL THE OTHERS TIMER
		reset_timer(0);
		disable_timer(0);
		
		showGameOver();
		
		//at last, disable itself
		reset_timer(1);
		disable_timer(1);
		LPC_TIM1->IR = 1;			/* clear interrupt flag */
		return;
		
		
	}
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void){
	
	if (scoreNeedsRedraw==1){
		updateScoreString();
		scoreNeedsRedraw=0;
		
	}
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
