#ifndef __GAME_H
#define __GAME_H

#include "../GLCD/GLCD.h"


//colors scheme
#define WALL_COLOR Blue
#define BACKGROUND_COLOR Black
#define STDPILL_COLOR Red
#define HOUSEDOOR_COLOR Magenta
#define PLAYER_COLOR Yellow

//position of maze start
#define MAZESTART 5

//player start position
#define START_XPOS 10
#define START_YPOS 15

//the screen size is 320x240. I divide these dimensions in cells so groups of pixels
//for now, cell size is 16x15 so we have 240/15 = 16 horizontal cells for each row, and 320/16 = 20 vertical cells for each column
//Real number of cells: 
#define XMAX 48  //80 //20
#define YMAX 64  //60 //16

#define CELL_H 5  //4 //16
#define CELL_W 5  //4 //15

//The labyrinth is mapped to a matrix of cells measuring 11x11

#define STD_PILLS 240
#define PWR_PILLS 6
/* integers for in-game objects:
		0 -> free cell (player can go through it)
		1 -> wall (player is blocked)
		2 -> standard pill
		3 -> power pill
		4 -> tele-transport tunnels (traversable by player to reach opposite parts of the map)
		7 -> house door
*/



/*
SCREEN TO MATRIX MAPPING:
Screen real size = 320x240 pixels
Cell size = 5x5 pixels -> a "cell" is a 5x5 square on the screen
H = 240/5 = 48 cells
V = 320 / 5 = 64 cells
Area: 48*64 = 3072 cells
240 pills + 6 power --> 2826 remaining cells
space for score, timer etc: (MAZESTART * XMAX) * 2 = 480
--> THE REMAINING 2826 - 480 = 2346 CELLS ARE FOR OTHER PROPS!
*/


static unsigned int maze[YMAX][XMAX] = {0};

//PACMAN -> quadrato 2x2 celle. Disegno pixel per pixel
static uint8_t player[16][16] = {
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, // Riga 1
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, // Riga 2
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // Riga 3
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}, // Riga 4
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 5
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // Riga 6
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, // Riga 7
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // Riga 8
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // Riga 9
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, // Riga 10
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 11
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}, // Riga 12
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}, // Riga 13
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // Riga 14
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, // Riga 15
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}  // Riga 16
};


//GFX
void placePills4();
void drawPills4();
void drawPills();
void drawWalls(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height);
void drawDoor(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height);
void drawTunnel(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height);
void drawBlanks();
void drawPlayer(uint16_t cellX, uint16_t cellY, uint8_t direction);
#endif