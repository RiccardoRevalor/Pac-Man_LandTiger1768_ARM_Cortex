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
#include <stdlib.h>

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
extern uint8_t life;
extern uint8_t gameTime;
char scoreS[5];
char gameTimeS[3];
char lifeS[2];
static uint8_t scoreNeedsRedraw = 0;
static uint8_t lifeNeedsRedraw = 0;
static uint16_t lastLifeIncrementGameTime = 0;

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

void updateLifeString(){
	sprintf(lifeS, "%d", life);
	GUI_Text(getPixelX(LIFECNT_X+10), getPixelY(LIFECNT_Y), (uint8_t *) lifeS, White, BACKGROUND_COLOR);
}



//PREVIOUS VALUE OF BLINKY CELL
uint8_t prevValue[4] = {0, 0, 0, 0}; //left up, left down, right up, right down)
//BLINKY START MOVES TO EXIT FROM THE HOUSE
uint8_t startMovesCnt = 0; //counter
uint8_t doStartMoves = 1; //1 -> do start moves routine, 0 -> routines finished, move normally
static uint8_t lastDir = UP_DIR; //last ghost dir
uint8_t frightened = 0; //frightened or not variable.0 -> frightened, 1 -> not frightened
uint8_t BlinkyDied = 0; //1 -> Blinky died, wait 3 sec for respawn in the house (and perform startMoves again)
void TIMER0_IRQHandler (void)
{
	//NVIC_DisableIRQ(TIMER0_IRQn);
	static uint8_t lastDir = RIGHT_DIR;
	static uint8_t isIdle = 0;
	char buf[100];
	
	if (isPaused == 1) {
		LPC_TIM0->IR = 1;			/* clear interrupt flag */
		return;
	}
	
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
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
            erasePlayer(plX, plY);
						erasePill(lookAheadX, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
			
						
        } else if (((maze[plY][lookAheadX] == STDPILL_CODE_1 || maze[plY][lookAheadX] == STDPILL_CODE_2) && maze[plY+1][lookAheadX] == FREE_CODE) || ((maze[plY+1][lookAheadX] == STDPILL_CODE_1 || maze[plY+1][lookAheadX] == STDPILL_CODE_2) && maze[plY][lookAheadX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
					
					//Score update
					score+=10;
					scoreNeedsRedraw=1;
					if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
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
						frightened = 1;
						scoreNeedsRedraw=1;
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
            erasePlayer(plX, plY);
						erasePill(lookAheadX, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
			
						
        } else if (((maze[plY][lookAheadX] == PWRPILL_CODE_1 || maze[plY][lookAheadX] == PWRPILL_CODE_2) && maze[plY+1][lookAheadX] == FREE_CODE) || ((maze[plY+1][lookAheadX] == PWRPILL_CODE_1 || maze[plY+1][lookAheadX] == PWRPILL_CODE_2) && maze[plY][lookAheadX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
					
					//Score update
					score+=50;
					frightened = 1;
					scoreNeedsRedraw=1;
					if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
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
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
            erasePlayer(plX, plY);
						erasePill(nextX-1, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
        } else if (((maze[plY][nextX] == STDPILL_CODE_1 || maze[plY][nextX] == STDPILL_CODE_2) && maze[plY+1][nextX] == FREE_CODE) || ((maze[plY+1][nextX] == STDPILL_CODE_1 || maze[plY+1][nextX] == STDPILL_CODE_2) && maze[plY][nextX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
				
					//Score update
					score+=10;
					scoreNeedsRedraw=1;
					if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
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
						frightened = 1;
						scoreNeedsRedraw=1;
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
            erasePlayer(plX, plY);
						erasePill(nextX-1, plY);
            drawPlayer(nextX, plY, playerDir, 1);
						plX = nextX;
        } else if (((maze[plY][nextX] == PWRPILL_CODE_1 || maze[plY][nextX] == PWRPILL_CODE_2) && maze[plY+1][nextX] == FREE_CODE) || ((maze[plY+1][nextX] == PWRPILL_CODE_1 || maze[plY+1][nextX] == PWRPILL_CODE_2) && maze[plY][nextX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
				
					//Score update
					score+=50;
					frightened = 1;
					scoreNeedsRedraw=1;
					if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
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
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
						erasePlayer(plX, plY);
						erasePill(plX, nextY-1);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[nextY][plX] == STDPILL_CODE_1 || maze[nextY][plX] == STDPILL_CODE_2) && maze[nextY][plX+1] == FREE_CODE) || ((maze[nextY][plX+1] == STDPILL_CODE_1 || maze[nextY][plX+1] == STDPILL_CODE_2) && maze[nextY][plX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
						
				
						//Score update
						score+=10;
						scoreNeedsRedraw=1;
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
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
						frightened = 1;
						scoreNeedsRedraw=1;
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
						erasePlayer(plX, plY);
						erasePill(plX, nextY-1);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[nextY][plX] == PWRPILL_CODE_1 || maze[nextY][plX] == PWRPILL_CODE_2) && maze[nextY][plX+1] == FREE_CODE) || ((maze[nextY][plX+1] == PWRPILL_CODE_1 || maze[nextY][plX+1] == PWRPILL_CODE_2) && maze[nextY][plX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
						
				
						//Score update
						score+=50;
						scoreNeedsRedraw=1;
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
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
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
						erasePlayer(plX, plY);
						erasePill(plX, lookAheadY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[lookAheadY][plX] == STDPILL_CODE_1 || maze[lookAheadY][plX] == STDPILL_CODE_2) && maze[lookAheadY][plX+1] == FREE_CODE) || ((maze[lookAheadY][plX+1] == STDPILL_CODE_1 || maze[lookAheadY][plX+1] == STDPILL_CODE_2) && maze[lookAheadY][plX] == FREE_CODE)) {
						//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
						
						//Score update
						score+=10;
						scoreNeedsRedraw=1;
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
				
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
						frightened = 1;
						scoreNeedsRedraw=1;
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
						erasePlayer(plX, plY);
						erasePill(plX, lookAheadY);
            drawPlayer(plX, nextY, playerDir, 1);
						plY = nextY;
        } else if (((maze[lookAheadY][plX] == PWRPILL_CODE_1 || maze[lookAheadY][plX] == PWRPILL_CODE_2) && maze[lookAheadY][plX+1] == FREE_CODE) || ((maze[lookAheadY][plX+1] == PWRPILL_CODE_1 || maze[lookAheadY][plX+1] == PWRPILL_CODE_2) && maze[lookAheadY][plX] == FREE_CODE)) {
						//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
						
						//Score update
						score+=50;
						frightened = 1;
						scoreNeedsRedraw=1;
						if (score >= LTHRES && (score % LTHRES == 0)) lifeNeedsRedraw=0; else lifeNeedsRedraw=1;
				
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
			if (score >= LTHRES && (score % LTHRES == 0) && lifeNeedsRedraw == 0) lifeNeedsRedraw=1; //block adding new life if in idle because it was already added the iteration before
			
			if (isIdle == 0) {
				erasePlayer(plX, plY);
				drawPlayer(plX, plY, lastDir, 0);
				isIdle = 1;
				
				if (scoreNeedsRedraw==1){
					updateScoreString();
					scoreNeedsRedraw=0;
				}
				
			}
		}
		
		lastDir = playerDir;
}
	
	//EXTRAPOINTS2: MOVE BLINKY
	//try every new cell in the four directions, is there's a wall reject it
	//prioprity: UP, LEFT, DOWN, RIGHT

	//TEST CELL UP:
	uint8_t bestDir = UP_DIR;
	int lowestDistance = 10000;
	int highestDistance = 0;
	int d = 0;
	
	//at first: do start moves routine
	if (BlinkyDied == 0) {
	if (doStartMoves == 1) {
		if (startMovesCnt < BLINKYSTATRTMOVES_COUNT) {
			bestDir = BlinkyStartMoves[startMovesCnt];
			++startMovesCnt;
		} else {
			//start moves routine finished, reset counters and set flag to zero
			doStartMoves = 0;
			startMovesCnt = 0;
		}
	}
	else {
		//move normally, start moves finished
	if (goodCellForBlinky(gX, gY, UP_DIR) == 1 && lastDir != DOWN_DIR) {
		//UP IS (gX, gY-1)
		//if yes, calculate manhattan and in case update the best dir and lowest distance
		d = manhattanDistance(plX, plY, gX, gY-1);
		if (frightened == 0) {
			if (d < lowestDistance) {
				bestDir = UP_DIR;
				lowestDistance = d;
			}
		} else {
			if (d > highestDistance) {
				bestDir = UP_DIR;
				highestDistance = d;
			}					
		}
	}
	if (goodCellForBlinky(gX, gY, LEFT_DIR) == 1 && lastDir != RIGHT_DIR) {
		//LEFT IS (gX-1, gY)
		//if yes, calculate manhattan and in case update the best dir and lowest distance
		d = manhattanDistance(plX, plY, gX-1, gY);
		if (frightened == 0) {
			if (d < lowestDistance) {
				bestDir = LEFT_DIR;
				lowestDistance = d;
			}
		} else {			
			if (d > highestDistance) {
				bestDir = LEFT_DIR;
				lowestDistance = d;
			}
		}
	}
	if (goodCellForBlinky(gX, gY, DOWN_DIR) == 1 && lastDir != UP_DIR) {
		//DOWN IS (gX, gY+1)
		//if yes, calculate manhattan and in case update the best dir and lowest distance
		d = manhattanDistance(plX, plY, gX, gY+1);
		if (frightened == 0 ) {
			if (d < lowestDistance) {
				bestDir = DOWN_DIR;
				lowestDistance = d;
			}
		} else {
			if (d > highestDistance) {
				bestDir = DOWN_DIR;
				highestDistance = d;
			}		
			
		}
	}
	if (goodCellForBlinky(gX, gY, RIGHT_DIR) == 1 && lastDir != LEFT_DIR) {
		//RIGHT IS (gX+1, gY)
		//if yes, calculate manhattan and in case update the best dir and lowest distance
		d = manhattanDistance(plX, plY, gX+1, gY);
		if (frightened == 0) {
			if (d < lowestDistance) {
				bestDir = RIGHT_DIR;
				lowestDistance = d;
			}
		} else {
			if (d > highestDistance) {
				bestDir = RIGHT_DIR;
				highestDistance = d;
			}		
			
		}
	}
}
	
	lastDir = bestDir; //update lastDir for the next iteration

	//At this Point I know the best direction for Blinky!
	eraseBlinky(gX, gY);
	

//save prevValues 
	//left up, left down, right up, right down
	prevValue[0] = maze[gY][gX];				//left up
	prevValue[1] = maze[gY+1][gX]; 			//left down
	prevValue[2] = maze[gY][gX+1]; 			//right up
	prevValue[3] = maze[gY+1][gX+1];		//right down


	//now: redraw blinky at the new cell and with the best choosen dir
	//if blinky was sitting on a pill, you have to redraw the pill too at the previous position
	//uint8_t prevValue[4] = {0, 0, 0, 0}; //left up, left down, right up, right down)
	//(gX, gY) point is in the left up cell


	if (prevValueIsCompletePill(prevValue, 0)){
			//Blinky was sitting on a complete stdpill, redraw it
			drawPills4(gX, gY, 0);
	} else if (prevValueIsCompletePill(prevValue, 1)){
			//Blinky was sitting on a complete pwrpill, redraw it
			drawPills4(gX, gY, 1);
	} else {
		//HALF PILL CASES
		
		//CASE 1: HALF PILL ON THE UPPER CELLS OF THE 2x2 square
		if (prevValue[0] == STDPILL_CODE_1 && prevValue[2] == STDPILL_CODE_2) {
			//half stdpill up
			drawPills4(gX, gY-1, 0);		
			//maze[gY+1][gX] = FREE_CODE;
			//maze[gY+1][gX+1] = FREE_CODE;
		} else if (prevValue[0] == STDPILL_CODE_2 && prevValue[2] == STDPILL_CODE_2){
			if (maze[gY-1][gX] == STDPILL_CODE_1) drawPills4(gX, gY-1, 0);
		}
		if (prevValue[0] == PWRPILL_CODE_1 && prevValue[2] == PWRPILL_CODE_2){
			//half pwrpill up
			drawPills4(gX, gY-1, 1);	
			//maze[gY+1][gX] = FREE_CODE;
			//maze[gY+1][gX+1] = FREE_CODE;
		} else if (prevValue[0] == PWRPILL_CODE_2 && prevValue[2] == PWRPILL_CODE_2){
			if (maze[gY-1][gX] == PWRPILL_CODE_1) drawPills4(gX, gY-1, 1);
		}
		
		//CASE 2: HALF PILL ON THE LOWER CELLS OF THE 2x2 square
		if (prevValue[1] == STDPILL_CODE_1 && prevValue[3] == STDPILL_CODE_2) {
			//half stdpill down
			drawPills4(gX, gY+1, 0);		
			//maze[gY][gX] = FREE_CODE;
			//maze[gY][gX+1] = FREE_CODE;
		} else if (prevValue[1] == STDPILL_CODE_2 && prevValue[3] == STDPILL_CODE_2) {
			if (maze[gX][gY] == STDPILL_CODE_1) drawPills4(gX, gY, 0);
		} 
		
		if (prevValue[1] == PWRPILL_CODE_1 && prevValue[3] == PWRPILL_CODE_2){
			//half pwrpill down
			drawPills4(gX, gY+1, 1);	
			//maze[gY][gX] = FREE_CODE;
			//maze[gY][gX+1] = FREE_CODE;
		}
		
		//CASE 3: HALF PILL ON THE RIGHTMOST CELLS OF THE 2x2 square
		
		if (prevValue[2] == STDPILL_CODE_1 && prevValue[3] == STDPILL_CODE_2) {
			//half stdpill right
			//drawPills4(gX+1, gY, 0);
			drawPills4(gX+1, gY, 0);
			//maze[gY][gX] = FREE_CODE;
			//maze[gY+1][gX] = FREE_CODE;			
		} 
		if (prevValue[0] == STDPILL_CODE_2 && prevValue[1] == STDPILL_CODE_2){
			if (maze[gY][gX-1] == STDPILL_CODE_1) drawPills4(gX-1, gY, 0);
		}
		
		
		if (prevValue[2] == PWRPILL_CODE_1 && prevValue[3] == PWRPILL_CODE_2){
			//half pwrpill right
			drawPills4(gX+1, gY, 1);	
			//maze[gY][gX] = FREE_CODE;
			//maze[gY+1][gX] = FREE_CODE;	
		}
		
		
		//CASE 4: HALF PILL ON THE LEFTMOST CELLS OF THE 2x2 square
		if (prevValue[0] == STDPILL_CODE_1 && prevValue[1] == STDPILL_CODE_2) {
			//half stdpill left
			drawPills4(gX-1, gY, 0);	
			//maze[gY][gX+1] = FREE_CODE;
			//maze[gY+1][gX+1] = FREE_CODE;			
		}
		if (prevValue[0] == PWRPILL_CODE_1 && prevValue[1] == PWRPILL_CODE_2){
			//half pwrpill left
			drawPills4(gX-1, gY, 1);	
			//maze[gY][gX+1] = FREE_CODE;1
			//maze[gY+1][gX+1] = FREE_CODE;	
		}
		
		
	}
	
	
	
    
	//choose bestdir and redraw ghost and save prev values
	switch (bestDir) {
        case UP_DIR:    gY--; break;
        case LEFT_DIR:  gX--; break;
        case DOWN_DIR:  gY++; break;
        case RIGHT_DIR: gX++; break;
    }
	
	
	//redraw Blinky ate the new (gX, gY)
	//redrawPills4();
	drawBlinky(gX, gY, bestDir, frightened);
	}	
		
	//check collision between Blinky and player
	if (checkCollision(gX, gY, plX, plY)){
		if (frightened == 0) {
		
			if (life == 1 ){
				showGameOver();
				deleteRemainingPills();
				isPaused = 1;
				firstGame = 1;
				remainingPills = STD_PILLS; //reset counter for remaining pills for the new game
				erasePlayer(plX, plY);
				doStartMoves = 1;
				gX = BLINKY_START_X;
				gY = BLINKY_STATRT_Y;
				frightened = 0;
			} else {
				life--;
				updateLifeString();
			}
		} else {
			//delete ghost
			doStartMoves = 1;
			BlinkyDied = 1;
			gX = BLINKY_START_X;
			gY = BLINKY_STATRT_Y;
			frightened = 0;
			
		}
		
	}

	
	
	
	
	
	
	
	

	
	if (score >= LTHRES && (score % LTHRES == 0)) {
    if (lifeNeedsRedraw == 0) {
        // Incrementa la vita e imposta il flag
        life = life + 1;
				score += 10;
        lifeNeedsRedraw = 1;
        updateLifeString();
    }
} /*else {
    // Resetta il flag quando il punteggio non è più un multiplo di 100
    lifeNeedsRedraw = 0;
}*/


	//CHECK REMAINING PILLS NUMBER TO DECLARE VICTORY
	if (remainingPills <= 0) {
		showVictory();
		//end of game: set isPaused to 1, set firstGame to 1
		isPaused = 1;
		firstGame = 1;
		remainingPills = STD_PILLS; //reset counter for remaining pills for the new game
		
		//reset the player, erasing its sprite from the maze
		erasePlayer(plX, plY);
	}


  LPC_TIM0->IR = 1;			/* clear interrupt flag */
	//NVIC_EnableIRQ(TIMER0_IRQn);
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
uint8_t countFrightened = 0;
uint8_t countBeforeRespawn = 0;
void TIMER1_IRQHandler (void)
{
	
	if (isPaused == 1) {
		LPC_TIM1->IR = 1;			/* clear interrupt flag */
		return;
	}
	
	
	/*
	
	TIMER 1
	Tasks:
	1) every n seconds, generate a power pill until we have a total of 6 powerpills (conuting even the already eaten ones) on the map
	2) every second decrement the 60 seconds time counter
	3) if the timer counter reaches 0, show GAME OVER Screen
	
	*/
	
	//count ten seconds for frightened mode then set frightened to zero
	++countFrightened;
	if (countFrightened >= 10) {
		frightened = 0;
		countFrightened = 0;
	}
	
	//if Blinky died, wait for 3 seconds after respwn in the middle of the house
	if (BlinkyDied == 1){
		++countBeforeRespawn;
		
		if (countBeforeRespawn >= 3) {
			countBeforeRespawn = 0;
			BlinkyDied = 0;
			drawBlinky(gX, gY,IDLE_DIR, 0);
			frightened = 0;
			countFrightened = 0;
		}
	}
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
				srand((plX+plY)*totalPills);
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
				showVictory();
				//end of game: set isPaused to 1, set firstGame to 1
				isPaused = 1;
				firstGame = 1;
				remainingPills = STD_PILLS; //reset counter for remaining pills for the new game
				
				//reset the player, erasing its sprite from the maze
				erasePlayer(plX, plY);
				
			}
		}
		
	} 
	
	//decrement gameTime by 1 time unit
	--gameTime;
	updateGameTimeString();
	
	if (gameTime <= 0) {
		
		
		//RESET AND DISABLE ALL THE OTHERS TIMER
		//reset_timer(0);
		//disable_timer(0);
		
		//end of game: set isPaused to 1, set firstGame to 1
		isPaused = 1;
		firstGame = 1;
		remainingPills = STD_PILLS; //reset counter for remaining pills for the new game
		erasePlayer(plX, plY);
		
		showGameOver();
		deleteRemainingPills();
		
		//at last, disable itself
		//reset_timer(1);
		//disable_timer(1);
		LPC_TIM1->IR = 1;
		return;
		
		
	}
	
	
	//send from CAN1 to CAN2 the struct containing gameTime, score, life
	CAN_Send();
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

extern uint8_t reproduceNotes;
extern uint8_t reproductionTime;
void TIMER2_IRQHandler (void){
	
	
	static uint8_t sineticks = 0; // Indice per la tabella
	
	if (reproduceNotes > 0){
		if (reproductionTime > 0) {
    uint16_t currentValue;

    // Ottieni il valore dalla tabella sinusoidale
    currentValue = SinTable[sineticks];
		currentValue = currentValue * 0.1;
    LPC_DAC->DACR = currentValue << 6; // Scrivi il valore al DAC (sinistra 6 bit)

    // Incrementa l'indice della tabella
    sineticks++;
    if (sineticks == 45) sineticks = 0; // Ritorna all'inizio della tabella
		--reproductionTime;
		} else {
			reproduceNotes = 0;
			reproductionTime = pillSoundReproductionCycles;
		}
	}
	
	LPC_TIM2->IR = 1; // Cancella il flag di interrupt
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
