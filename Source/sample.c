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
#include "timer/timer.h"
#include "../../game.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


/* General variables for the game */

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();
	
	//TO DO: POTENZIOMETRO COME SEED DEL RAND()
	
  //TP_Init();
	//TouchPanel_Calibrate();
	
	LCD_Clear(BACKGROUND_COLOR);
	//GUI_Text(0, 280, (uint8_t *) " touch here : 1 sec to clear  ", Red, White);
	
	
	//LCD_DrawLine(0, 0, 200, 200, White);
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	//init_timer(0, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */

	
	
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
	
	

	// Muri interni
	
	/* inizio muri interni */
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
	
	
	/* fine muri interni */
	
	
	//PILLS MANAGEMENT DA FARE ALLA FINE
	placePills();
	drawPills();
	
	
	//left tunnel
	drawTunnel(0, (YMAX - 2*MAZESTART) / 2, 2, 8);
	//right tunnel
	drawTunnel(XMAX - 2, (YMAX - 2*MAZESTART) / 2, 2, 8);
	
	
	
	
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
