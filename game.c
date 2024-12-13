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
		
   //autodistance();
	 int sum = countFreeCells();
	 float prob =  246.0 / (float) sum;
	 int standardPills = STD_PILLS;
	 int powerPills = PWR_PILLS;
	
	 while (standardPills > 0){
		 for (y = 0; y < YMAX - 1 && standardPills > 0; y+=2) {
					for (x = 0; x < XMAX - 1 && standardPills > 0; x+=2) {
						
						//if (maze[y][x] != 0) continue;
						if (maze[y][x] != FREE_CODE || maze[y][x + 1] != FREE_CODE || maze[y + 1][x] != FREE_CODE || maze[y + 1][x + 1] != FREE_CODE) continue;
						//if (!isAreaFree(y,x,1)) continue;
						float probCell = (float)rand()/(float)(RAND_MAX);
						
						if (probCell < prob / 2){
							//maze[y][x] = 2; //put a new std pill here
							maze[y][x] = STDPILL_CODE_1;       // Top-left
              maze[y][x + 1] = STDPILL_CODE_2;   // Top-right
              maze[y + 1][x] = STDPILL_CODE_2;   // Bottom-left
              maze[y + 1][x + 1] = STDPILL_CODE_2; // Bottom-right
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

void placePills4(){
	/*
	Random average placement:
	count the number of free cells (having value = 0)
	the probability is 246 (number of std pills + pwr pills) / free_cells
	use rand for every cell, if the returened value of rand() is > of the probability, place the pill in that cell, otherwise skip it
	*/

	 int y;
	 int x;
	 int standardPills = STD_PILLS;
	 int powerPills = PWR_PILLS;
	
	 while (standardPills > 0){
		 for (y = MAZESTART; y <= YMAX - MAZESTART - 1 && standardPills > 0; y++) {
					for (x = 1; x <= XMAX - 1 && standardPills > 0; x++) {
						
						//if (maze[y][x] != 0) continue; //|| maze[y + 2][x + 1] != 0
						if (maze[y][x] != FREE_CODE || maze[y][x + 1] != FREE_CODE || maze[y][x-1] != FREE_CODE || maze[y][x+2] != FREE_CODE || maze[y + 1][x] != FREE_CODE || maze[y + 1][x + 1] != FREE_CODE || maze[y + 1][x + 2] != FREE_CODE) continue;
						//if (!isAreaFree(y,x,1)) continue;
						float probCell = (float)rand()/(float)(RAND_MAX);
						
						//if (1 == 1){
							//maze[y][x] = 2; //put a new std pill here
							maze[y][x] = STDPILL_CODE_1;       // Top-left
              maze[y][x + 1] = STDPILL_CODE_2;   // Top-right
              maze[y + 1][x] = STDPILL_CODE_2;   // Bottom-left
              maze[y + 1][x + 1] = STDPILL_CODE_2; // Bottom-right
							standardPills --;
						//}
						
						
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
	for (y = MAZESTART; y < YMAX - MAZESTART; y++){
		for (x = 0; x < XMAX; x++){
			if (maze[y][x] == STDPILL_CODE_1){
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
        //LCD_DrawLine(x_right, y_right, x_bottom, y_bottom, STDPILL_COLOR); // Lato destro-inferiore
        //LCD_DrawLine(x_bottom, y_bottom, x_left, y_left, STDPILL_COLOR); // Lato inferiore-sinistro
        LCD_DrawLine(x_left, y_left, x_top, y_top, STDPILL_COLOR); // Lato sinistro-superiore
			}
		}
	}
}

void drawPills4(){
	int y, x;
	for (y = MAZESTART; y < YMAX - MAZESTART; y++){
		for (x = 0; x < XMAX; x++){
			if (maze[y][x] == STDPILL_CODE_1){
				//pills su quadrato di celle 2x2
				//cioè 16x16 pixel
				
				
			/*
				//estremo in alto
			uint16_t x0 = x * CELL_W;
      uint16_t y0 = y * CELL_H;
      uint16_t x1 = x0 + 2 * CELL_W - 1;
      uint16_t y1 = y0 + 2 * CELL_H - 1;
			
			LCD_DrawLine(x0, y0, x1, y0, STDPILL_COLOR); // Linea superiore
      LCD_DrawLine(x0, y0, x0, y1, STDPILL_COLOR); // Linea sinistra
      LCD_DrawLine(x1, y0, x1, y1, STDPILL_COLOR); // Linea destra
      LCD_DrawLine(x0, y1, x1, y1, STDPILL_COLOR); // Linea inferiore
			*/
				
			 // Coordinate del centro del blocco 2x2
                uint16_t cx = (x * CELL_W + (x + 1) * CELL_W) / 2; // Media tra le celle orizzontali
                uint16_t cy = (y * CELL_H + (y + 1) * CELL_H) / 2; // Media tra le celle verticali

                // Coordinate dei vertici del rombo
                uint16_t x_top = cx;
                uint16_t y_top = cy - CELL_H / 2; // Sopra di metà altezza del blocco
                uint16_t x_right = cx + CELL_W / 2; // A destra di metà larghezza del blocco
                uint16_t y_right = cy;
                uint16_t x_bottom = cx;
                uint16_t y_bottom = cy + CELL_H / 2; // Sotto di metà altezza del blocco
                uint16_t x_left = cx - CELL_W / 2; // A sinistra di metà larghezza del blocco
                uint16_t y_left = cy;

                // Disegna il rombo
                LCD_DrawLine(x_top, y_top, x_right, y_right, STDPILL_COLOR);  // Lato superiore-destro
                LCD_DrawLine(x_right, y_right, x_bottom, y_bottom, STDPILL_COLOR); // Lato destro-inferiore
                LCD_DrawLine(x_bottom, y_bottom, x_left, y_left, STDPILL_COLOR); // Lato inferiore-sinistro
                LCD_DrawLine(x_left, y_left, x_top, y_top, STDPILL_COLOR);  // Lato sinistro-superiore
              
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
			
			if (cellValue == WALL_CODE){
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
			maze[y][x] = WALL_CODE;
		}
	}
}

void drawBlanks(){
	uint8_t x, y;
	for (y = MAZESTART; y < YMAX - MAZESTART; y++) {
		for (x = 0; x < YMAX; x++){
			if (maze[y][x] != 0) continue;
			uint16_t x0 = x * CELL_W;
      uint16_t y0 = y * CELL_H;
      uint16_t x1 = x0 + CELL_W - 1;
      uint16_t y1 = y0 + CELL_H - 1;
			
			LCD_DrawLine(x0, y0, x1, y0, White); // Linea superiore
      LCD_DrawLine(x0, y0, x0, y1, White); // Linea sinistra
      LCD_DrawLine(x1, y0, x1, y1, White); // Linea destra
      LCD_DrawLine(x0, y1, x1, y1, White); // Linea inferiore
			
		}
	}
}


void drawDoor(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height){
	uint8_t x, y;
	for (y = yS; y < yS + height; y++) {
		for (x = xS; x < xS + width; x++){
			uint16_t x0 = x * CELL_W;
      uint16_t y0 = y * CELL_H;
      uint16_t x1 = x0 + CELL_W - 1;
      uint16_t y1 = y0 + CELL_H - 1;
			
			LCD_DrawLine(x0, y0, x1, y0, HOUSEDOOR_COLOR); // Linea superiore
      LCD_DrawLine(x0, y0, x0, y1, HOUSEDOOR_COLOR); // Linea sinistra
      LCD_DrawLine(x1, y0, x1, y1, HOUSEDOOR_COLOR); // Linea destra
      LCD_DrawLine(x0, y1, x1, y1, HOUSEDOOR_COLOR); // Linea inferiore
			
			
			//save 7 in the maze matrix
			maze[y][x] = DOOR_CODE;
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
			maze[y][x] = TUNNEL_CODE;
		}
	}
	
}


void rotatePlayer(uint8_t dest[PLAYER_H][PLAYER_W], uint8_t dir) {
	/* Function to perform a rotation on the player matrix
	If dir == 0 (right dir), no rotation needed, so no need to call this function !
	This works wether dir = 1 or 2 or 3
	*/
	
	uint8_t x, y;
	
	if (dir == RIGHT_DIR) return;
	
	uint8_t dimY = PLAYER_H;
	uint8_t dimX = PLAYER_W;
	
	if (dir == LEFT_DIR) {
		//rotate the matrix by 180 degress clockwise
		for (y = 0; y < dimY; y++) {
         for (x = 0; x < dimX; x++) {
              dest[dimY - 1 - y][dimX - 1 - x] = player[y][x];
         }
		}
	}
	
	if (dir == DOWN_DIR) {
		//rotate the matrix by 90 degress clockwise
		for (y = 0; y < dimY; y++) {
         for (x = 0; x < dimX; x++) {
              dest[x][dimY - 1 - y] = player[y][x];
         }
		}
	}
	
	if (dir == UP_DIR) {
		//rotate the matrix by 270 degress clockwise
		for (y = 0; y < dimY; y++) {
         for (x = 0; x < dimX; x++) {
              dest[dimX - 1 - x][y] = player[y][x];
         }
		}
	}
	
	
	
	
}


uint16_t getPixelX(uint16_t cellX) { return cellX * CELL_W - 1;}; //cellX / XMAX;
uint16_t getPixelY(uint16_t cellY) { return cellY * CELL_H - 1;}; //(cellY + MAZESTART) / YMAX;

void drawPlayer(uint16_t cellX, uint16_t cellY, uint8_t direction){
	/*
	direction: 
	0 -> right
	*/
	uint16_t xplayer = getPixelX(cellX), yplayer = getPixelY(cellY);
	uint16_t x, y;
	uint16_t cntX = 0, cntY = 0;
	
	
	/*
	Operations:
	1) set the player number in the maze matrix to assign the cells to the player prop
		 (cellY, cellX) is the left-upper cell of the square of the player
	2) draw the player shape by iterating on the player's own matrix
	*/
	
	maze[cellY][cellX] = PLAYER_CODE;
	maze[cellY+1][cellX] = PLAYER_CODE;
	maze[cellY][cellX+1] = PLAYER_CODE;
	maze[cellY+1][cellX+1] = PLAYER_CODE;
	
	
	static uint8_t playerRotated[PLAYER_H][PLAYER_W];
	//if direction != right, player needs to be rotated first
	if (direction != RIGHT_DIR) rotatePlayer(playerRotated, direction);
	
	if (direction == RIGHT_DIR) {
			for (x = xplayer; x < xplayer + PLAYER_CELLS_W * CELL_W; x++) {
				for (y = yplayer; y < yplayer + PLAYER_CELLS_H * CELL_H; y++) {
					if (player[cntY][cntX] == 1) {
						LCD_SetPoint(x, y, PLAYER_COLOR);	//set the player colore in the ones of the matrix to draw the player's shape
					} else {
						LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
					}
					cntY++;
				}
				cntY = 0;
				++cntX;
			}
	} else {
		for (x = xplayer; x < xplayer + PLAYER_CELLS_W * CELL_W; x++) {
				for (y = yplayer; y < yplayer + PLAYER_CELLS_H * CELL_H; y++) {
					if (playerRotated[cntY][cntX] == 1) {
						LCD_SetPoint(x, y, PLAYER_COLOR);	//set the player colore in the ones of the matrix to draw the player's shape
					} else {
						LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
					}
					cntY++;
				}
				cntY = 0;
				++cntX;
		}
	}

}

void erasePlayer(uint16_t cellX, uint16_t cellY){
	uint16_t xplayer = getPixelX(cellX), yplayer = getPixelY(cellY);
	uint16_t x, y;
	uint16_t cntX = 0, cntY = 0;
	
	maze[cellY][cellX] = FREE_CODE;
	maze[cellY+1][cellX] = FREE_CODE;
	maze[cellY][cellX+1] = FREE_CODE;
	maze[cellY+1][cellX+1] = FREE_CODE;
	
	for (x = xplayer; x < xplayer + PLAYER_CELLS_W * CELL_W; x++) {
				for (y = yplayer; y < yplayer + PLAYER_CELLS_H * CELL_H; y++) {
					LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
					
					cntY++;
				}
				cntY = 0;
				++cntX;
	}
}

	
	