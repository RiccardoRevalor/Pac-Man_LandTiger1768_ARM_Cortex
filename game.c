#include "game.h"
#include "stdlib.h"
#include <stdbool.h>

int countFreeCells() {
	int sum = 0;
	int x, y;
	
	for (y = 0; y < YMAX; y++){
		for (x = 0; x < XMAX; x++){
			if (maze[y][x] == 0) ++sum;
			
		}
	}
	
	return sum;
}

bool isAreaFree(int startY, int startX, int range) {
    // Controlla che le celle in un'area di range x range attorno al blocco siano libere
		int y, x;
    for (y = startY - range; y <= startY + range; y++) {
        for (x = startX - range; x <= startX + range; x++) {
            if (y >= 0 && y < YMAX && x >= 0 && x < XMAX) { // Limita l'area ai confini della matrice
                if (maze[y][x] != 0) {
                    return false; // Trovata una cella occupata
                }
            }
        }
    }
    return true; // Tutte le celle nell'area sono libere
}


void autodistance(){
	int y;
	int x;
	
	/*
	for (y = 5; y < 40; y++) {
			for (x = 0; x < XMAX; x++) {
					maze[y][x] = 9;

			}
		}
	
	*/
	
		
	for (y = 0; y < YMAX; y++) {
			for (x = 0; x < XMAX - 1; x+=7) {
				maze[y][x] = 9;
			}
	}
					
}
void placePills(){
	/*
	Random average placement:
	count the number of free cells (having value = 0)
	the probability is 246 (number of std pills + pwr pills) / free_cells
	use rand for every cell, if the returened value of rand() is > of the probability, place the pill in that cell, otherwise skip it
	*/

	 int y;
	 int x;
		
   autodistance();
	 int sum = countFreeCells();
	 float prob =  246.0 / (float) sum;
	 int standardPills = 240;
	 int powerPills = PWR_PILLS;
	
	 while (standardPills > 0){
		 for (y = 0; y < YMAX - 1 && standardPills > 0; y+=2) {
					for (x = 0; x < XMAX - 1 && standardPills > 0; x+=2) {
						
						//if (maze[y][x] != 0) continue;
						if (maze[y][x] != 0 || maze[y][x + 1] != 0 || maze[y + 1][x] != 0 || maze[y + 1][x + 1] != 0) continue;
						//if (!isAreaFree(y,x,1)) continue;
						float probCell = (float)rand()/(float)(RAND_MAX);
						
						if (probCell < prob / 2){
							//maze[y][x] = 2; //put a new std pill here
							maze[y][x] = 2;       // Top-left
              maze[y][x + 1] = 2;   // Top-right
              maze[y + 1][x] = 2;   // Bottom-left
              maze[y + 1][x + 1] = 2; // Bottom-right
							standardPills --;
						}
						
						
						/* before: fill everything algorithm
							if (maze[y][x] == 0 && standardPills > 0) {
									maze[y][x] = 2; // Pillola standard
									standardPills--;
							}
						*/
					}
			}
		}

    while (powerPills > 0) {
        int x = rand() % XMAX;
        int y = rand() % YMAX;

        if (maze[y][x] == 0) { // Sostituisci pillola standard con power pill
            maze[y][x] = 3;
            powerPills--;
        }
    }
}

void drawPills(){
	int y, x;
	for (y = 0; y < YMAX; y++){
		for (x = 0; x < XMAX; x++){
			if (maze[y][x] == 2){
				//ho celle 4x4
// Coordinate del centro della cella
        uint16_t cx = x * CELL_W + CELL_W / 2;
        uint16_t cy = y * CELL_H + CELL_H / 2;

       // Vertici del rombo
        uint16_t x_top = cx;
        uint16_t y_top = cy - CELL_H / 4;
        uint16_t x_right = cx + CELL_W / 4;
        uint16_t y_right = cy;
        uint16_t x_bottom = cx;
        uint16_t y_bottom = cy + CELL_H / 4;
        uint16_t x_left = cx - CELL_W / 4;
        uint16_t y_left = cy;

        // Disegna il rombo
        LCD_DrawLine(x_top, y_top, x_right, y_right, STDPILL_COLOR); // Lato superiore-destro
        LCD_DrawLine(x_right, y_right, x_bottom, y_bottom, STDPILL_COLOR); // Lato destro-inferiore
        LCD_DrawLine(x_bottom, y_bottom, x_left, y_left, STDPILL_COLOR); // Lato inferiore-sinistro
        LCD_DrawLine(x_left, y_left, x_top, y_top, STDPILL_COLOR); // Lato sinistro-superiore
			}
		}
	}
}



void drawPerimeterWalls(){
	int y;
	int x;
	for (y = 0; y < YMAX; y++){
		for (x = 0; x < XMAX; x++){
			
			int cellValue = maze[y][x];
			
			if (cellValue == 1){
				//wall detected
				//translate fake cell coordinates into the real ones
				//each cell is 16x15 pixels
				uint16_t x0 = x * CELL_W;
        uint16_t y0 = y * CELL_H;
        uint16_t x1 = x0 + CELL_W - 1;
        uint16_t y1 = y0 + CELL_H - 1;
				
				// Disegna il rettangolo per rappresentare il muro
        LCD_DrawLine(x0, y0, x1, y0, WALL_COLOR); // Linea superiore
        LCD_DrawLine(x0, y0, x0, y1, WALL_COLOR); // Linea sinistra
        LCD_DrawLine(x1, y0, x1, y1, WALL_COLOR); // Linea destra
        LCD_DrawLine(x0, y1, x1, y1, WALL_COLOR); // Linea inferiore

			}
			
		}
	}
}


void drawWalls(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height){
	uint8_t x, y;
	for (y = yS; y < yS + height; y++) {
		for (x = xS; x < xS + width; x++){
			uint16_t x0 = x * CELL_W;
      uint16_t y0 = y * CELL_H;
      uint16_t x1 = x0 + CELL_W - 1;
      uint16_t y1 = y0 + CELL_H - 1;
			
			LCD_DrawLine(x0, y0, x1, y0, WALL_COLOR); // Linea superiore
      LCD_DrawLine(x0, y0, x0, y1, WALL_COLOR); // Linea sinistra
      LCD_DrawLine(x1, y0, x1, y1, WALL_COLOR); // Linea destra
      LCD_DrawLine(x0, y1, x1, y1, WALL_COLOR); // Linea inferiore
			
			
			//save 1 in the maze matrix
			maze[y][x] = 1;
		}
	}
}


void drawTunnel(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height){
	uint8_t x, y;
	for (y = yS; y < yS + height; y++) {
		for (x = xS; x < xS + width; x++){
			uint16_t x0 = x * CELL_W;
      uint16_t y0 = y * CELL_H;
      uint16_t x1 = x0 + CELL_W - 1;
      uint16_t y1 = y0 + CELL_H - 1;
			
			LCD_DrawLine(x0, y0, x1, y0, BACKGROUND_COLOR); // Linea superiore
      LCD_DrawLine(x0, y0, x0, y1, BACKGROUND_COLOR); // Linea sinistra
      LCD_DrawLine(x1, y0, x1, y1, BACKGROUND_COLOR); // Linea destra
      LCD_DrawLine(x0, y1, x1, y1, BACKGROUND_COLOR); // Linea inferiore
			
			
			//save 1 in the maze matrix
			maze[y][x] = 4;
		}
	}
	
}