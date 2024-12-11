#include "game.h"


void placePills(){
	 int y;
	 int x;
	 unsigned int standardPills = STD_PILLS;
	 unsigned int powerPills = PWR_PILLS;
	 for (y = 0; y < YMAX; y++) {
        for (x = 0; x < XMAX; x++) {
            if (maze[y][x] == 0 && standardPills > 0) {
                maze[y][x] = 2; // Pillola standard
                standardPills--;
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