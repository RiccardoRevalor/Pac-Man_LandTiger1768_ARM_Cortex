#include "game.h"
#include "stdlib.h"
#include <stdbool.h>
#include <stdio.h>

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
						
						//If (y,x) is in the ghosts house -> don't spawn pills in there!
						
						if ((y == 28 || y == 29 || y == 30) && (x >= 20 && x <= 27)) continue; //ghosts house coordinates
						

						if (maze[y][x] != FREE_CODE || maze[y][x + 1] != FREE_CODE || maze[y + 1][x] != FREE_CODE || maze[y + 1][x + 1] != FREE_CODE) continue;
						//if (!isAreaFree(y,x,1)) continue;
						float probCell = (float)rand()/(float)(RAND_MAX);
						
						if (probCell < prob / 2){
							maze[y][x] = STDPILL_CODE_1;       // Top-left
              maze[y][x + 1] = STDPILL_CODE_2;   // Top-right
              maze[y + 1][x] = STDPILL_CODE_2;   // Bottom-left
              maze[y + 1][x + 1] = STDPILL_CODE_2; // Bottom-right
							
							
							standardPills --;
						}
						
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
	 int pwrPillCounter = 0;
	 int standardPills = STD_PILLS;
	 int powerPills = PWR_PILLS;
	
	 while (standardPills > 0){
		 for (y = MAZESTART; y <= YMAX - MAZESTART - 1 && standardPills > 0; y++) {
					for (x = 1; x <= XMAX - 1 && standardPills > 0; x++) {
						
						//If (y,x) is in the ghosts house -> don't spawn pills in there!					
						if ((y == 28 || y == 29 || y == 30) && (x >= 20 && x <= 27)) continue; //ghosts house coordinates
						
						//if x == 1 || x == XMAX-1 and y = tunnels y -> don't put pills at the entrance of tunnels
						if ((x == 1 || x == XMAX-1 || x == 2 || x == XMAX -2 || x == XMAX -3) && (y >= T_Y && y < T_Y + T_HEIGTH)) continue;
						
						
						//if (maze[y][x] != 0) continue; //|| maze[y + 2][x + 1] != 0 || maze[y + 1][x + 2] || maze[y][x+2] != FREE_CODE
						//prima c'era: maze[y][x-1] != FREE_CODE
						if (maze[y][x] != FREE_CODE || maze[y][x + 1] != FREE_CODE || maze[y + 1][x] != FREE_CODE || maze[y + 1][x + 1] != FREE_CODE ) continue;
						//if (!isAreaFree(y,x,1)) continue;
						float probCell = (float)rand()/(float)(RAND_MAX);
				
						maze[y][x] = STDPILL_CODE_1;       // Top-left
						
            maze[y][x + 1] = STDPILL_CODE_2;   // Top-right
            maze[y + 1][x] = STDPILL_CODE_2;   // Bottom-left
            maze[y + 1][x + 1] = STDPILL_CODE_2; // Bottom-right
						
						
						drawPills4(x, y, 0);
						
						standardPills --;
					}
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

/*
void drawPills4(){
	int y, x;
	for (y = MAZESTART; y < YMAX - MAZESTART; y++){
		for (x = 0; x < XMAX; x++){
			if (maze[y][x] == STDPILL_CODE_1){
				//pills su quadrato di celle 2x2
				//cioè 16x16 pixel
				
				
				//estremo in alto
			uint16_t x0 = x * CELL_W;
      uint16_t y0 = y * CELL_H;
      uint16_t x1 = x0 + 2 * CELL_W - 1;
      uint16_t y1 = y0 + 2 * CELL_H - 1;
			
			LCD_DrawLine(x0, y0, x1, y0, STDPILL_COLOR); // Linea superiore
      LCD_DrawLine(x0, y0, x0, y1, STDPILL_COLOR); // Linea sinistra
      LCD_DrawLine(x1, y0, x1, y1, STDPILL_COLOR); // Linea destra
      LCD_DrawLine(x0, y1, x1, y1, STDPILL_COLOR); // Linea inferiore
			
				
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
*/


void drawPills4(uint16_t cellX, uint16_t cellY, uint8_t isPowerPill) {
    uint16_t xcell = getPixelX(cellX); // Converte la posizione in celle nei pixel X
    uint16_t ycell = getPixelY(cellY); // Converte la posizione in celle nei pixel Y
    uint16_t px, py;

    // Cicla sulla matrice stdPill e disegna i pixel corrispondenti
    for (px = 0; px < STDPILLS_W; px++) {
        for (py = 0; py < STDPILLS_H; py++) {
            if (stdPill[py][px] == 1) {
                // Disegna il pixel con il colore della pillola
								if (isPowerPill == 0) {
									LCD_SetPoint(xcell + px, ycell + py, STDPILL_COLOR);
								} else {
									LCD_SetPoint(xcell + px, ycell + py, PWRPILL_COLOR);
								}
            } else {
                // Disegna il pixel con il colore dello sfondo
                LCD_SetPoint(xcell + px, ycell + py, BACKGROUND_COLOR);
            }
        }
    }
}




void drawMapWalls(){
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

void drawBlank(uint16_t xS, uint16_t yS){
	uint16_t x0 = xS * CELL_W;
      uint16_t y0 = yS * CELL_H;
      uint16_t x1 = x0 + CELL_W - 1;
      uint16_t y1 = y0 + CELL_H - 1;
	
	LCD_DrawLine(x0, y0, x1, y0, White); // Linea superiore
      LCD_DrawLine(x0, y0, x0, y1, White); // Linea sinistra
      LCD_DrawLine(x1, y0, x1, y1, White); // Linea destra
      LCD_DrawLine(x0, y1, x1, y1, White); // Linea inferiore
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


uint16_t getPixelX(uint16_t cellX) { return cellX * CELL_W;}; //cellX / XMAX;
uint16_t getPixelY(uint16_t cellY) { return cellY * CELL_H;}; //(cellY + MAZESTART) / YMAX;

void drawPlayer(uint16_t cellX, uint16_t cellY, uint8_t direction, uint8_t animation){
	/*
	direction: 
	0 -> right
	*/
	uint16_t xplayer = getPixelX(cellX), yplayer = getPixelY(cellY);
	uint16_t x, y;
	uint16_t cntX = 0, cntY = 0;
	uint16_t res = (cellY + cellX) % 2;
	
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
					if (res == 0 || animation == 0) {
						if (player[cntY][cntX] == 1) {
							if (LCD_GetPoint(x, y ) != PLAYER_COLOR) LCD_SetPoint(x, y, PLAYER_COLOR);	//set the player colore in the ones of the matrix to draw the player's shape
						} else {
							if (LCD_GetPoint(x, y ) != BACKGROUND_COLOR) LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
						}
					} else {
						if (playerEating[cntY][cntX] == 1) {
							if (LCD_GetPoint(x, y ) != PLAYER_COLOR) LCD_SetPoint(x, y, PLAYER_COLOR);	//set the player colore in the ones of the matrix to draw the player's shape
						} else {
							if (LCD_GetPoint(x, y ) != BACKGROUND_COLOR) LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
						}
					}
					cntY++;
				}
				cntY = 0;
				++cntX;
			}
	} else {
		for (x = xplayer; x < xplayer + PLAYER_CELLS_W * CELL_W; x++) {
				for (y = yplayer; y < yplayer + PLAYER_CELLS_H * CELL_H; y++) {
					if (res == 0 || animation == 0) {
						if (playerRotated[cntY][cntX] == 1) {
							if (LCD_GetPoint(x, y ) != PLAYER_COLOR) LCD_SetPoint(x, y, PLAYER_COLOR);	//set the player colore in the ones of the matrix to draw the player's shape
						} else {
							if (LCD_GetPoint(x, y ) != BACKGROUND_COLOR) LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
						}
					} else {
						if (playerEating[cntY][cntX] == 1) {
							if (LCD_GetPoint(x, y ) != PLAYER_COLOR) LCD_SetPoint(x, y, PLAYER_COLOR);	//set the player colore in the ones of the matrix to draw the player's shape
						} else {
							if (LCD_GetPoint(x, y ) != BACKGROUND_COLOR) LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
						}
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

extern uint8_t reproduceNotes;
void erasePill(uint16_t cellX, uint16_t cellY){
	uint16_t xplayer = getPixelX(cellX), yplayer = getPixelY(cellY);
	uint16_t x, y;
	uint16_t cntX = 0, cntY = 0;
	
	maze[cellY][cellX] = FREE_CODE;
	maze[cellY+1][cellX] = FREE_CODE;
	maze[cellY][cellX+1] = FREE_CODE;
	maze[cellY+1][cellX+1] = FREE_CODE;
	
	//remove the stdpill from the pills matrix as well
	//pillsCells[cellY][cellX] = FREE_CODE;
	
	for (x = xplayer; x < xplayer + PLAYER_CELLS_W * CELL_W; x++) {
				for (y = yplayer; y < yplayer + PLAYER_CELLS_H * CELL_H; y++) {
					LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
					
					cntY++;
				}
				cntY = 0;
				++cntX;
	}
	
	remainingPills--; //UPDATE REMAINING PILLS
	
	reproduceNotes = 1;
	
}


void showGameOver(){
	
	
	
	LCD_Clear(BACKGROUND_COLOR);
		
		
		
		
		
	/*
	uint16_t x, y;
	uint16_t cntX = 0, cntY = 0;
	uint16_t xplayer = getPixelX(0), yplayer = getPixelY(MAZESTART + 10);
	for (x = xplayer; x < xplayer + XMAX * CELL_W; x++) {
				for (y = yplayer; y < YMAX * CELL_H; y++) {
					if (gameOverGfxMap[y][x] != 0) {
						LCD_SetPoint(x, y, White); //set the background color in the zeros of the player matrix to create space from other props
					}
					cntY++;
				}
				cntY = 0;
				++cntX;
	}
	*/
	int y;
	int x;
	for (y = 0; y < YMAX; y++){
		for (x = 0; x < XMAX; x++){
			
			int cellValue = gameOverGfxMap[y][x];
			
			if (cellValue == WALL_CODE){
				//wall detected
				//translate fake cell coordinates into the real ones
				//each cell is 16x15 pixels
				uint16_t x0 = x * CELL_W;
        uint16_t y0 = y * CELL_H;
        uint16_t x1 = x0 + CELL_W - 1;
        uint16_t y1 = y0 + CELL_H - 1;
				
				// Disegna il rettangolo per rappresentare il muro
        LCD_DrawLine(x0, y0, x1, y0, White); // Linea superiore
        LCD_DrawLine(x0, y0, x0, y1, White); // Linea sinistra
        LCD_DrawLine(x1, y0, x1, y1, White); // Linea destra
        LCD_DrawLine(x0, y1, x1, y1, White); // Linea inferiore

			}
			
		}
	}
	GUI_Text(getPixelX(XMAX / 2 - 7), getPixelY(2), (uint8_t *) "GAME OVER!", Red, BACKGROUND_COLOR);
	GUI_Text(getPixelX(0), getPixelY(5), (uint8_t *) "Press INT0 to start a new game", Red, BACKGROUND_COLOR);
	reset_RIT();

	
}

void deleteRemainingPills(){
	int x, y;
	for (y = MAZESTART; y < YMAX - MAZESTART; y++){
		for (x = 0; x < XMAX; x++) {
			if (maze[y][x] == STDPILL_CODE_1 || maze[y][x] == PWRPILL_CODE_1) {
				//upper left cell of a stdpill/pwrpill -> delete the whole pill from the maze
				maze[y][x] = FREE_CODE;
				maze[y+1][x] = FREE_CODE;
				maze[y][x+1] = FREE_CODE;
				maze[y+1][x+1] = FREE_CODE;
			}
		}
	}
	
}

extern uint8_t EINT0_down;
void newGameRoutine() {
	LCD_Clear(BACKGROUND_COLOR);
	GUI_Text(getPixelX(SCORE_X), getPixelY(SCORE_Y), (uint8_t *) "SCORE:", White, BACKGROUND_COLOR);
	GUI_Text(getPixelX(SCORE_X+10), getPixelY(SCORE_Y), (uint8_t *) "0000", White, BACKGROUND_COLOR);
	GUI_Text(getPixelX(LIFECNT_X), getPixelY(LIFECNT_Y), (uint8_t *) "Lives:", White, BACKGROUND_COLOR);
	GUI_Text(getPixelX(LIFECNT_X+10), getPixelY(LIFECNT_Y), (uint8_t *) "1", White, BACKGROUND_COLOR);
	GUI_Text(getPixelX(TIMECNT_X), getPixelY(TIMECNT_Y), (uint8_t *) "Time:", White, BACKGROUND_COLOR);
	GUI_Text(getPixelX(TIMECNT_X+8), getPixelY(TIMECNT_Y), (uint8_t *) "60", White, BACKGROUND_COLOR);
	
	//set score to 0, life to 1, power pills counter to 0, gameTime to GAMETIME_LIMIT
	score = 0;
	life = 1;
	pwrPillsCounter = 0;
	gameTime = GAMETIME_LIMIT;
	
	//set default player position
	plX = XMAX / 2 - 1;
	plY = YMAX / 2;
	
	drawMapWalls();
	
	
	//DRAW PLAYER AT START POSITION (AT THE CENTER, DOWN AFTER THE HOUSE)
	drawPlayer(plX , plY, RIGHT_DIR, 0);
	
	//debug
	//drawBlank(plX, plY);
	
	//SET DIR AS IDLE (DON'T MOVE UNLESS THE USERS TOUCHES THE JOYSTICK)
	playerDir = IDLE_DIR;
	
	
	//left tunnel
	drawTunnel(RT_X, T_Y, T_WIDTH, T_HEIGTH);
	//right tunnel
	drawTunnel(LT_X, T_Y, T_WIDTH, T_HEIGTH);
	
	//PILLS MANAGEMENT
	placePills4();
	
	
	//draw door
	drawDoor(24, 28, 1, 1);
	//drawBlanks();
	
	//draw blinky
	drawBlinky(gX, gY, IDLE_DIR, 0);
	
	//START GAME TIMERS
	
	/*
	disable_RIT();
	reset_RIT();
	init_RIT(RIT_Time);
	enable_RIT();
	*/
	reset_RIT();
	
	disable_timer(0);
	disable_timer(1);
	reset_timer(0);
	reset_timer(1);
	
	
	//START TIMER0 TO UPDATE GAME (60 FPS)
	if (DEBUG_MOVS == 1) {
		init_timer(0, FPS_Time_DEBUG);
	} else {
		init_timer(0, FPS_Time);
	}
	
	init_timer(1, TimeCounter_Time);
	//init_timer(2, TextRedraw_Time);
	
	//playerDir = RIGHT_DIR;
	disable_timer(0);
	disable_timer(1);
	reset_timer(0);
	reset_timer(1);
	enable_timer(0);
	enable_timer(1);
	
	/*
	NVIC_EnableIRQ(EINT0_IRQn);							 
	LPC_PINCON->PINSEL4  |= (1 << 20);     
	EINT0_down = 0;
	*/
	
	reset_RIT();
}

uint32_t TIM0Val, TIM1Val;
uint8_t lastDir; 
uint16_t lastPlX, lastPlY;
uint8_t lastAnimation;
extern uint16_t lastScore;
extern uint16_t lastLife;

void redrawPills4(){
	//iterate trhough maze and redraw the remaining stdpills/pwrpills
	int x,y;
	for (y = MAZESTART; y < YMAX - MAZESTART; y++){
		for (x = 0; x < XMAX; x++) {
			if (maze[y][x] == STDPILL_CODE_1){
				//redraw the stdpill
				drawPills4(x,y,0);
			} 
			
			if (maze[y][x] == PWRPILL_CODE_1) {
				//redraw the pwrpill
				drawPills4(x,y,1);
				
				
			}
			
		}
	}
	
}

extern char scoreS[5];
extern char gameTimeS[3];
extern char lifeS[2];


void resumeGameRoutine() {
	LCD_Clear(BACKGROUND_COLOR);
	
	
	GUI_Text(getPixelX(SCORE_X), getPixelY(SCORE_Y), (uint8_t *) "SSCORE:", White, BACKGROUND_COLOR);
	sprintf(scoreS, "%04d", score);
	GUI_Text(getPixelX(SCORE_X+10), getPixelY(SCORE_Y), (uint8_t *) scoreS, White, BACKGROUND_COLOR);
	GUI_Text(getPixelX(LIFECNT_X), getPixelY(LIFECNT_Y), (uint8_t *) "Lives:", White, BACKGROUND_COLOR);
	sprintf(lifeS, "%d", life);
	GUI_Text(getPixelX(LIFECNT_X+10), getPixelY(LIFECNT_Y), (uint8_t *) lifeS, White, BACKGROUND_COLOR);
	GUI_Text(getPixelX(TIMECNT_X), getPixelY(TIMECNT_Y), (uint8_t *) "Time:", White, BACKGROUND_COLOR);
	GUI_Text(getPixelX(TIMECNT_X+8), getPixelY(TIMECNT_Y), (uint8_t *) "  ", White, BACKGROUND_COLOR);
	
	drawMapWalls();
	
	
	//DRAW PLAYER AT START POSITION (AT THE CENTER, DOWN AFTER THE HOUSE)
	drawPlayer(lastPlX , lastPlY, lastDir, lastAnimation);
	
	//debug
	//drawBlank(plX, plY);
	
	//SET DIR AS IDLE (DON'T MOVE UNLESS THE USERS TOUCHES THE JOYSTICK)
	playerDir = lastDir;
	
	
	//left tunnel
	drawTunnel(RT_X, T_Y, T_WIDTH, T_HEIGTH);
	//right tunnel
	drawTunnel(LT_X, T_Y, T_WIDTH, T_HEIGTH);
	
	//PILLS MANAGEMENT
	//Now I have to redraw the remaining (= not eaten) pills, not generate new ones
	redrawPills4();
	
	//drawBlanks();
	
	
	//START GAME TIMERS
	
	
	disable_RIT();
	reset_RIT();
	init_RIT(RIT_Time);
	enable_RIT();
	
	
	//START TIMER0 TO UPDATE GAME
	/*
	init_timer(0, TIM0Val);
	
	init_timer(1, TIM1Val);
	//init_timer(1, TextRedraw_Time);
	
	//playerDir = RIGHT_DIR;
	reset_timer(0);
	reset_timer(1);
	enable_timer(0);
	enable_timer(1);
	*/
	
	NVIC_EnableIRQ(EINT0_IRQn);							 
	LPC_PINCON->PINSEL4  |= (1 << 20);     
	EINT0_down = 0;
	reset_RIT();
	
	
	
}

void showPause(uint8_t canResume) {
	if (canResume == 0) {
		LCD_Clear(BACKGROUND_COLOR);
		GUI_Text(getPixelX(XMAX / 2 - 10), getPixelY(YMAX / 2 -1), (uint8_t *) "Game Paused", Blue, BACKGROUND_COLOR);
		GUI_Text(getPixelX(0), getPixelY(YMAX / 2 +3), (uint8_t *) "Press INT0 to start a new game", Blue, BACKGROUND_COLOR);
		
	} else {		
		LCD_Clear(BACKGROUND_COLOR);
		GUI_Text(getPixelX(XMAX / 2 - 10), getPixelY(YMAX / 2 -1), (uint8_t *) "Game Paused", Blue, BACKGROUND_COLOR);
		GUI_Text(getPixelX(5), getPixelY(YMAX / 2 +3), (uint8_t *) "Press INT0 to resume", Blue, BACKGROUND_COLOR);
		//pause game timers
		//save all the game variables!
		TIM0Val = LPC_TIM0->TC;
		TIM1Val = LPC_TIM1->TC;
		lastDir = playerDir;
		lastPlX = plX;
		lastPlY = plY;
		if (lastDir == IDLE_DIR) lastAnimation = 0; else lastAnimation = 1;
		/*
		disable_timer(0);
		disable_timer(1);
		*/
		reset_RIT();
		NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
		LPC_PINCON->PINSEL4  |= (1 << 20);     /* External interrupt 0 pin selection */
		EINT0_down = 0;
		reset_RIT();
	}
}

void showVictory() {
	LCD_Clear(Green);
	GUI_Text(getPixelX(XMAX / 2 - 7), getPixelY(YMAX / 2 -1), (uint8_t *) "Victory!", Blue, BACKGROUND_COLOR);
	GUI_Text(getPixelX(0), getPixelY(YMAX / 2 +3), (uint8_t *) "Press INT0 to start a new game", Blue, BACKGROUND_COLOR);
	reset_RIT();

}

int manhattanDistance(uint8_t playerX, uint8_t playerY, uint8_t newGhostX, uint8_t newGhostY){
	return abs(newGhostX-playerX)+abs(newGhostY-playerY);
	
}

uint8_t checkCollision(uint8_t blinkyX, uint8_t blinkyY, uint8_t playerX, uint8_t playerY) {
    // Controllo dell'intersezione tra le aree di Blinky (2x2) e del giocatore (2x2)
    if ((blinkyX < playerX + 2) && (blinkyX + 2 > playerX) &&
        (blinkyY < playerY + 2) && (blinkyY + 2 > playerY)) {
        return 1; // Collisione rilevata
    }
    return 0; // Nessuna collisione
}


uint8_t goodCellForBlinky(uint8_t x, uint8_t y, uint8_t direction) {
    // Controlla che Blinky non esca dai limiti della mappa
    if (x < 0 || x >= XMAX - 1 || y < MAZESTART || y >= YMAX - 2) {
        return 0; // Fuori dai limiti
    }

    // Controlla la direzione del movimento
    if (direction == RIGHT_DIR) {
        // Spostamento a destra (controlla le celle a destra del quadrato)
        if ((maze[y][x + 2] == PLAYER_CODE || maze[y][x + 2] == FREE_CODE || maze[y][x + 2] == STDPILL_CODE_1 || maze[y][x + 2] == STDPILL_CODE_2 || 
             maze[y][x + 2] == PWRPILL_CODE_1 || maze[y][x + 2] == PWRPILL_CODE_2) &&
            (maze[y + 1][x + 2] == PLAYER_CODE || maze[y + 1][x + 2] == FREE_CODE || maze[y + 1][x + 2] == STDPILL_CODE_1 || maze[y + 1][x + 2] == STDPILL_CODE_2 || 
             maze[y + 1][x + 2] == PWRPILL_CODE_1 || maze[y + 1][x + 2] == PWRPILL_CODE_2)) {
            return 1; // Direzione valida
        }
    } else if (direction == LEFT_DIR) {
        // Spostamento a sinistra (controlla le celle a sinistra del quadrato)
        if ((maze[y][x - 1] == PLAYER_CODE || maze[y][x - 1] == FREE_CODE || maze[y][x - 1] == STDPILL_CODE_1 || maze[y][x - 1] == STDPILL_CODE_2 || 
             maze[y][x - 1] == PWRPILL_CODE_1 || maze[y][x - 1] == PWRPILL_CODE_2) &&
            (maze[y + 1][x - 1] == PLAYER_CODE || maze[y + 1][x - 1] == FREE_CODE || maze[y + 1][x - 1] == STDPILL_CODE_1 || maze[y + 1][x - 1] == STDPILL_CODE_2 || 
             maze[y + 1][x - 1] == PWRPILL_CODE_1 || maze[y + 1][x - 1] == PWRPILL_CODE_2)) {
            return 1; // Direzione valida
        }
    } else if (direction == UP_DIR) {
        // Spostamento verso l'alto (controlla le celle sopra il quadrato)
        if ((maze[y - 1][x] == PLAYER_CODE || maze[y - 1][x] == FREE_CODE || maze[y - 1][x] == STDPILL_CODE_1 || maze[y - 1][x] == STDPILL_CODE_2 || 
             maze[y - 1][x] == PWRPILL_CODE_1 || maze[y - 1][x] == PWRPILL_CODE_2) &&
            (maze[y - 1][x + 1] == PLAYER_CODE || maze[y - 1][x + 1] == FREE_CODE || maze[y - 1][x + 1] == STDPILL_CODE_1 || maze[y - 1][x + 1] == STDPILL_CODE_2 || 
             maze[y - 1][x + 1] == PWRPILL_CODE_1 || maze[y - 1][x + 1] == PWRPILL_CODE_2)) {
            return 1; // Direzione valida
        }
    } else if (direction == DOWN_DIR) {
        // Spostamento verso il basso (controlla le celle sotto il quadrato)
        if ((maze[y + 2][x] == PLAYER_CODE || maze[y + 2][x] == FREE_CODE || maze[y + 2][x] == STDPILL_CODE_1 || maze[y + 2][x] == STDPILL_CODE_2 || 
             maze[y + 2][x] == PWRPILL_CODE_1 || maze[y + 2][x] == PWRPILL_CODE_2) &&
            (maze[y + 2][x + 1] == PLAYER_CODE || maze[y + 2][x + 1] == FREE_CODE || maze[y + 2][x + 1] == STDPILL_CODE_1 || maze[y + 2][x + 1] == STDPILL_CODE_2 || 
             maze[y + 2][x + 1] == PWRPILL_CODE_1 || maze[y + 2][x + 1] == PWRPILL_CODE_2)) {
            return 1; // Direzione valida
        }
    }

    return 0; // Direzione non valida
}




uint8_t goodCellForBlinky2(uint8_t x, uint8_t y, uint8_t direction){
	//1 -> good cell
	//0 -> bas cell, cannot choose this direction for Blinky!
	
	//(x,y) is the UPPER LEFTMOST CELL OF the 2x2 ghost
	
	if (x <= 0 || x >= XMAX - 1) return 0;
	if (y <= MAZESTART || y >= YMAX - MAZESTART - 1) return 0;
	
	if (direction == RIGHT_DIR) {
				int lookAheadX = x + 2; 
        int nextX = y + 1; 
        if (maze[y][lookAheadX] == FREE_CODE && maze[y+1][lookAheadX] == FREE_CODE) {
            // Il giocatore può muoversiS
            return 1;
        } else if ((maze[y][lookAheadX] == STDPILL_CODE_1 || maze[y][lookAheadX] == STDPILL_CODE_2) && (maze[y+1][lookAheadX] == STDPILL_CODE_1 || maze[y+1][lookAheadX] == STDPILL_CODE_2)) {
            return 1;
			
						
        } else if (((maze[y][lookAheadX] == STDPILL_CODE_1 || maze[y][lookAheadX] == STDPILL_CODE_2) && maze[y+1][lookAheadX] == FREE_CODE) || ((maze[y+1][lookAheadX] == STDPILL_CODE_1 || maze[y+1][lookAheadX] == STDPILL_CODE_2) && maze[y][lookAheadX] == FREE_CODE)) {
					return 1;
				
				} else if ((maze[y][lookAheadX] == PWRPILL_CODE_1 || maze[y][lookAheadX] == PWRPILL_CODE_2) && (maze[y+1][lookAheadX] == PWRPILL_CODE_1 || maze[y+1][lookAheadX] == PWRPILL_CODE_2)) {
            return 1;
			
						
        } else if (((maze[y][lookAheadX] == PWRPILL_CODE_1 || maze[y][lookAheadX] == PWRPILL_CODE_2) && maze[y+1][lookAheadX] == FREE_CODE) || ((maze[y+1][lookAheadX] == PWRPILL_CODE_1 || maze[y+1][lookAheadX] == PWRPILL_CODE_2) && maze[y][lookAheadX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
					
					return 1;
				
				}
				return 0;
  }
	
	if (direction == LEFT_DIR) {
		int nextX = x - 1;
		if (maze[y][nextX] == FREE_CODE && maze[y+1][nextX] == FREE_CODE) {
						//PURE CHUNK OF 2x2 FREE BLOCKS
            return 1;	
        } else if ((maze[y][nextX] == STDPILL_CODE_1 || maze[y][nextX] == STDPILL_CODE_2) && (maze[y+1][nextX] == STDPILL_CODE_1 || maze[y+1][nextX] == STDPILL_CODE_2)) {
						//PURE CHUNK OF 2x2 STDPILL BLOCKS
            return 1;
        } else if (((maze[y][nextX] == STDPILL_CODE_1 || maze[y][nextX] == STDPILL_CODE_2) && maze[y+1][nextX] == FREE_CODE) || ((maze[y+1][nextX] == STDPILL_CODE_1 || maze[y+1][nextX] == STDPILL_CODE_2) && maze[y][nextX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
				
					return 1;
				
				} else if ((maze[y][nextX] == PWRPILL_CODE_1 || maze[y][nextX] == PWRPILL_CODE_2) && (maze[y+1][nextX] == PWRPILL_CODE_1 || maze[y+1][nextX] == PWRPILL_CODE_2)) {
						//PURE CHUNK OF 2x2 PWRPILL BLOCKS
            return 1;
        } else if (((maze[y][nextX] == PWRPILL_CODE_1 || maze[y][nextX] == PWRPILL_CODE_2) && maze[y+1][nextX] == FREE_CODE) || ((maze[y+1][nextX] == PWRPILL_CODE_1 || maze[y+1][nextX] == PWRPILL_CODE_2) && maze[y][nextX] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK				
					return 1;				
				}
				return 0;		
	}
	
	if (direction == UP_DIR) {
		int nextY = y - 1; // Spostamento in alto
        if (maze[nextY][x] == FREE_CODE && maze[nextY][x+1] == FREE_CODE) {
            return 1;
        } else if ((maze[nextY][x] == STDPILL_CODE_1 || maze[nextY][x] == STDPILL_CODE_2) && (maze[nextY][x+1] == STDPILL_CODE_1 || maze[nextY][x+1] == STDPILL_CODE_2)) {
            
						return 1;
        } else if (((maze[nextY][x] == STDPILL_CODE_1 || maze[nextY][x] == STDPILL_CODE_2) && maze[nextY][x+1] == FREE_CODE) || ((maze[nextY][x+1] == STDPILL_CODE_1 || maze[nextY][x+1] == STDPILL_CODE_2) && maze[nextY][x] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
						
				
						return 1;
				
				} else if ((maze[nextY][x] == PWRPILL_CODE_1 || maze[nextY][x] == PWRPILL_CODE_2) && (maze[nextY][x+1] == PWRPILL_CODE_1 || maze[nextY][x+1] == PWRPILL_CODE_2)) {
            return 1;
        } else if (((maze[nextY][x] == PWRPILL_CODE_1 || maze[nextY][x] == PWRPILL_CODE_2) && maze[nextY][x+1] == FREE_CODE) || ((maze[nextY][x+1] == PWRPILL_CODE_1 || maze[nextY][x+1] == PWRPILL_CODE_2) && maze[nextY][x] == FREE_CODE)) {
					//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
						
				
						return 1;
				}
				return 0;
	}
	
	if (direction == DOWN_DIR){
		
		int lookAheadY = y + 2;
        int nextY = y + 1; // Spostamento in basso
        if (maze[lookAheadY][x] == FREE_CODE && maze[lookAheadY][x+1] == FREE_CODE) {
            return 1;
        } else if ((maze[lookAheadY][x] == STDPILL_CODE_1 || maze[lookAheadY][x] == STDPILL_CODE_2) && (maze[lookAheadY][x+1] == STDPILL_CODE_1 || maze[lookAheadY][x+1] == STDPILL_CODE_2)) {
            
						return 1;
        } else if (((maze[lookAheadY][x] == STDPILL_CODE_1 || maze[lookAheadY][x] == STDPILL_CODE_2) && maze[lookAheadY][x+1] == FREE_CODE) || ((maze[lookAheadY][x+1] == STDPILL_CODE_1 || maze[lookAheadY][x+1] == STDPILL_CODE_2) && maze[lookAheadY][x] == FREE_CODE)) {
						//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND STDPILL BLOCK
						
						return 1;
				
				} else if ((maze[lookAheadY][x] == PWRPILL_CODE_1 || maze[lookAheadY][x] == PWRPILL_CODE_2) && (maze[lookAheadY][x+1] == PWRPILL_CODE_1 || maze[lookAheadY][x+1] == PWRPILL_CODE_2)) {
            //FULL PWR PILL
						//Score update
						return 1;
        } else if (((maze[lookAheadY][x] == PWRPILL_CODE_1 || maze[lookAheadY][x] == PWRPILL_CODE_2) && maze[lookAheadY][x+1] == FREE_CODE) || ((maze[lookAheadY][x+1] == PWRPILL_CODE_1 || maze[lookAheadY][x+1] == PWRPILL_CODE_2) && maze[lookAheadY][x] == FREE_CODE)) {
						//HYBRID CHUNK SPLITTED BETWEEN E FREE BLOCK AND PWRPILL BLOCK
						
						return 1;
			}
			return 0;
		
	}
	
	
	return 0;
}
void drawBlinky(uint16_t cellX, uint16_t cellY, uint8_t direction, uint8_t animation){
	uint16_t xplayer = getPixelX(cellX), yplayer = getPixelY(cellY);
	uint16_t x, y;
	uint16_t cntX = 0, cntY = 0;
	uint16_t res = (cellY + cellX) % 2;
	
	
	maze[cellY][cellX] = PLAYER_CODE;
	maze[cellY+1][cellX] = PLAYER_CODE;
	maze[cellY][cellX+1] = PLAYER_CODE;
	maze[cellY+1][cellX+1] = PLAYER_CODE;
	
	

			for (x = xplayer; x < xplayer + PLAYER_CELLS_W * CELL_W; x++) {
				for (y = yplayer; y < yplayer + PLAYER_CELLS_H * CELL_H; y++) {
					if (res == 0 || animation == 0) {
						if (Blinky[cntY][cntX] == 1) {
							if (LCD_GetPoint(x, y ) != BLINKY_COLOR) LCD_SetPoint(x, y, BLINKY_COLOR);	//set the player colore in the ones of the matrix to draw the player's shape
						} else {
							if (LCD_GetPoint(x, y ) != BACKGROUND_COLOR) LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
						}
					} else {
						if (Blinky[cntY][cntX] == 1) {
							if (LCD_GetPoint(x, y ) != BLINKY_COLOR) LCD_SetPoint(x, y, BLINKY_COLOR);	//set the player colore in the ones of the matrix to draw the player's shape
						} else {
							if (LCD_GetPoint(x, y ) != BACKGROUND_COLOR) LCD_SetPoint(x, y, BACKGROUND_COLOR); //set the background color in the zeros of the player matrix to create space from other props
						}
					}
					cntY++;
				}
				cntY = 0;
				++cntX;
			}
	} 

extern CAN_msg CAN_TxMsg;
uint8_t CAN_Send() {
	// send from CAN1 to CAN2
	/*
	CAN PAYLOAD:
	gameTime -> 8 bits
	life -> 8 bits
	score -> 16 bits (8+8)
	*/
	
	CAN_TxMsg.data[0] = gameTime; 
	CAN_TxMsg.data[1] = life;
	CAN_TxMsg.data[2] = (score >> 8) & 0xFF;			//send first 8 MSB of score
	CAN_TxMsg.data[3] = score & 0xFF; 						//send last 8 bits of score
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 2;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg (1, &CAN_TxMsg);               /* transmit message */
	
	return 1;
}

uint8_t prevValueIsCompletePill(uint8_t prevV[4], uint8_t pwrPill) {
	if (pwrPill == 0 && prevV[0] == STDPILL_CODE_1 && prevV[1] == STDPILL_CODE_2 && prevV[2] == STDPILL_CODE_2 && prevV[3] == STDPILL_CODE_2) return 1;
	if (pwrPill == 0 && prevV[0] == PWRPILL_CODE_1 && prevV[1] == PWRPILL_CODE_2 && prevV[2] == PWRPILL_CODE_2 && prevV[3] == PWRPILL_CODE_2) return 1;

	
	return 0;
	
}
void create_valid_nodes(Node nodes[], int* node_count) {
		//put all the free cells or the cells hosting the stdpill/pwr pill as nodes in the graph
    *node_count = 0;
		uint8_t x,y;
    for (y = 0; y < YMAX; y++) {
        for (x = 0; x < XMAX; x++) {
					if (maze[y][x] == FREE_CODE && maze[y][x+1] == FREE_CODE && maze[y+1][x] == FREE_CODE && maze[y+1][x+1] == FREE_CODE) {
                Node node = { .x = x, .y = y, .g = 0, .h = 0, .f = 0, .parent = NULL };
                nodes[*node_count] = node;
                (*node_count)++;
            }
					if (maze[y][x] == STDPILL_CODE_1 && maze[y][x+1] == STDPILL_CODE_2 && maze[y+1][x] == STDPILL_CODE_2 && maze[y+1][x+1] == STDPILL_CODE_2) {
                Node node = { .x = x, .y = y, .g = 0, .h = 0, .f = 0, .parent = NULL };
                nodes[*node_count] = node;
                (*node_count)++;
            }
					
        }
    }
}





	
	