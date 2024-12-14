/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h" //TIMER 
#include "../../game.h"  //GAME LIBRARY
#include "../../RIT.h"   //RIT 


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


/* General variables for the game */
unsigned int maze[YMAX][XMAX] = {0};
uint8_t playerDir;
uint16_t plX = XMAX / 2;
uint16_t plY = YMAX / 2 + 2;

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
	
	
  LCD_Initialization();
	
	//TO DO: POTENZIOMETRO COME SEED DEL RAND()
	
  //TP_Init();
	//TouchPanel_Calibrate();
	
	LCD_Clear(BACKGROUND_COLOR);
	
	
	//draw perimeter walls
	
	drawWalls(0, MAZESTART, XMAX, 2);														//upper perimeter
	drawWalls(0, YMAX - MAZESTART - 2, XMAX, 2);								//lower perimeter
	drawWalls(0, MAZESTART, 2, YMAX - 2*MAZESTART);							//left perimeter
	drawWalls(XMAX - 2, MAZESTART, 2, YMAX - 2*MAZESTART); 			//right perimeter
	
	//CASETTA IN MEZZO
	drawWalls(XMAX / 2 - 4, YMAX / 2, 8, 2); //base  sotto
	drawWalls(XMAX / 2 - 6, YMAX / 2 - 4, 2, 6); //lato sx
	drawWalls(XMAX / 2 + 4, YMAX / 2 - 4, 2, 6); //lato dx
	drawWalls(XMAX / 2 - 6, YMAX / 2 - 4, 4, 2); //anta pre-porta sx
	drawWalls(XMAX / 2 + 2, YMAX / 2 - 4, 2, 2);		//anta pre-porta dx
	drawDoor(XMAX / 2 - 2, YMAX / 2 - 4, 4, 1);			//porta
	
	//drawWalls();
	//drawWalls(38, YMAX /2 - 5, 12, 2);

	
	//NUOVA MAPPA
	drawWalls(24, MAZESTART + 2, 2, 5);
	drawWalls(13, MAZESTART + 5, 7, 2);
	drawWalls(6, MAZESTART + 5, 3, 2);
	drawWalls(6, MAZESTART + 11, 3, 8);
  drawWalls(13, MAZESTART + 11, 2, 6);
	drawWalls(13, MAZESTART + 17, 6, 2);
	drawWalls(19, MAZESTART + 11, 11, 2);
	drawWalls(23, MAZESTART + 13, 3, 6);
	drawWalls(30, MAZESTART + 5, 7, 2);
	drawWalls(41, MAZESTART + 5, 1, 2);
	drawWalls(34, MAZESTART + 11, 8, 3);
	drawWalls(34, MAZESTART + 18, 8, 3);
	drawWalls(13, YMAX - MAZESTART - 8, 7, 2);
	drawWalls(6, YMAX - MAZESTART - 8, 3, 2);
	drawWalls(24, YMAX - MAZESTART - 8, 3, 2);
	drawWalls(36, YMAX - MAZESTART - 8, 6, 2);
	drawWalls(6, YMAX - MAZESTART - 14, 14, 2);
	drawWalls(24, YMAX - MAZESTART - 14, 10, 2);
	drawWalls(6, YMAX - MAZESTART - 20, 2, 6);
	drawWalls(XMAX - 2 - 4, YMAX - MAZESTART - 20, 4, 2);
	drawWalls(XMAX - (XMAX /4) - 2, (YMAX - 2*MAZESTART) / 2 - 4, 2, 8);
	drawWalls(XMAX /4, (YMAX - 2*MAZESTART) / 2 + 6, 2, 8);
	
	/*
	// Muri interni
	

	//sottoquadrante alto
  drawWalls(XMAX / 2 - 1, MAZESTART + 2, 2, 8);
	drawWalls(XMAX / 2 - 1 - 4, MAZESTART + 12, 10, 2);
	drawWalls(XMAX / 2 - 1 - 4 + 4, MAZESTART + 14, 2, 6);
	
	drawWalls(XMAX /4 - 6 , MAZESTART + 4, 4, 6);
	drawWalls(XMAX /4 , MAZESTART + 4, 6, 6);
	
	drawWalls(XMAX - (XMAX /4) - 6, MAZESTART + 4, 4, 6);
	drawWalls(XMAX - (XMAX /4), MAZESTART + 4, 6, 6);
	
	drawWalls(XMAX - (XMAX /4) - 5, MAZESTART + 12, 10, 2);
	drawWalls(XMAX /4 - 6, MAZESTART + 12, 10, 2);
	
	drawWalls(XMAX - (XMAX / 2) + 1, MAZESTART + 2, 2, 8);
	drawWalls(XMAX - (XMAX / 2) + 1 - 4, MAZESTART + 12, 10, 2);
	drawWalls(XMAX - (XMAX / 2) + 1 - 4 + 4, MAZESTART + 14, 2, 6);
	
	//sottoquadrante basso
	drawWalls(XMAX /4 - 6 , YMAX - 2*MAZESTART - 5, 4, 6);
	drawWalls(XMAX /4 , YMAX - 2*MAZESTART - 5, 6, 6);
	
	drawWalls(XMAX - (XMAX /4) - 6, YMAX - 2*MAZESTART - 5, 4, 6);
	drawWalls(XMAX - (XMAX /4), YMAX - 2*MAZESTART - 5, 6, 6);
	
	drawWalls(XMAX - (XMAX /4) - 5, YMAX - 2*MAZESTART - 13, 10, 2);
	drawWalls(XMAX /4 - 6, YMAX - 2*MAZESTART - 13, 10, 2);
	
	
	drawWalls(XMAX / 2 - 1, YMAX - 2*MAZESTART - 3, 4, 8);
	drawWalls(XMAX / 2 - 1 - 4, YMAX - 2*MAZESTART - 13, 10, 2);
	//drawWalls(XMAX / 2 - 1 - 4 + 4, MAZESTART + 14, 2, 6);
	
	drawWalls(XMAX /4 - 2, (YMAX - 2*MAZESTART) / 2 - 6, 2, 8);
	drawWalls(XMAX /4 - 2, (YMAX - 2*MAZESTART) / 2 + 6, 2, 8);
	
	drawWalls(XMAX - (XMAX /4) - 2, (YMAX - 2*MAZESTART) / 2 - 6, 2, 8);
	drawWalls(XMAX - (XMAX /4) - 2, (YMAX - 2*MAZESTART) / 2 + 6, 2, 8);
	
	*/
	
	
	//DRAW PLAYER AT START POSITION (AT THE CENTER, DOWN AFTER THE HOUSE)
	drawPlayer(plX , plY, RIGHT_DIR);
	
	//debug
	//drawBlank(plX, plY);
	
	//SET DIR AS IDLE (DON'T MOVE UNLESS THE USERS TOUCHES THE JOYSTICK)
	playerDir = IDLE_DIR;
	
	//PILLS MANAGEMENT DA FARE ALLA FINE
	//placePills4();
	//drawPills4();
	
	
	//left tunnel
	drawTunnel(0, (YMAX - 2*MAZESTART) / 2, 2, 8);
	//right tunnel
	drawTunnel(XMAX - 2, (YMAX - 2*MAZESTART) / 2, 2, 8);
	
	//drawBlanks();
	
	
	//ENABLE JOYSTICK
	joystick_init();
	
	//START GAME TIMERS
	
	//START RIT FOR POLLING THE JOYSTICK
	init_RIT(RIT_Time);
	
	//START TIMER0 TO UPDATE GAME (60 FPS)
	if (DEBUG_MOVS == 1) {
		init_timer(0, FPS_Time_DEBUG);
	} else {
		init_timer(0, FPS_Time);
	}
	
	reset_RIT();
	enable_RIT();
	enable_timer(0);
	
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
