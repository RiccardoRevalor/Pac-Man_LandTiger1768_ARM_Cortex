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
	
	//enable_timer(0);
	//drawPerimeterWalls();
	//LCD_DrawLine(100, 100, 150, 150, Red);
	
	
	//draw perimeter walls
	drawWalls(0, 0, YMAX, 4);	//upper perimeter
	drawWalls(0, XMAX - 4, YMAX, 4); //lower perimeter
	drawWalls(0, 0, 4, XMAX); //left perimeter
	drawWalls(YMAX - 4, 0, 4, XMAX);
	
	// Muri interni
  drawWalls(YMAX / 2, 4, 2, 20);      // Corridoio superiore
	
	drawWalls(10, 10, 12, 10);	
	drawWalls(10, 27, 12, 4);
	drawWalls(YMAX / 2 + 10, 27, 12, 4);
  drawWalls(10, XMAX / 2 + 10, 12, 4);
	drawWalls(YMAX - 20, 4, 12, 4);
	drawWalls(YMAX - 20, XMAX - 20, 25, 8);
	
	//CASETTA IN MEZZO
	drawWalls(YMAX /2 + 5, 30, 2, 8);
	drawWalls(YMAX /2 - 5, 30, 2, 8);
	drawWalls(YMAX /2 - 5, 38, 12, 2);
	
	
	drawWalls(YMAX /2, 38, 12, 2);
	drawWalls(YMAX /2, 38, 2, 10);

	
	
	
	//random walls in the center of the screen
	
	
	//PILLS MANAGEMENT DA FARE ALLA FINE
	placePills();
	drawPills();
	
	
	//left tunnel
	drawTunnel(0, XMAX / 2 - 4, 4, 8);
	//right tunnel
	drawTunnel(YMAX - 4, XMAX / 2 - 4, 4, 8);
	
	
	
	
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
