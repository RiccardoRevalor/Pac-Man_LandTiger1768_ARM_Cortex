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

//Player's dimensions in terms of pixels
//The player occupied a square of 2x2 cells, so it would be 10x10 pixels since each cell is 5x5
#define PLAYER_CELLS_H 2
#define PLAYER_CELLS_W 2

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
		9 -> player
*/
#define FREE_CODE 0
#define WALL_CODE 1
#define STDPILL_CODE_1 2
#define STDPILL_CODE_2 10
#define TUNNEL_CODE 4
#define DOOR_CODE 7
#define PLAYER_CODE 9

//PLAYER DIRECTION
/*
0 -> right
1 -> left
2 -> up
3 -> down
4 -> idle 
default direction (at te beginning of the game) is right
*/
#define RIGHT_DIR 0
#define LEFT_DIR 1
#define UP_DIR 2
#define DOWN_DIR 3
#define IDLE_DIR 4
extern uint8_t playerDir;


//PLAYER POSITION
//these are the default position at te beginning of a new game
static uint16_t plX = XMAX / 2;
static uint16_t plY = YMAX / 2 + 2;


//IN-GAME TIMINGS
#define RIT_Time 0x004C4B40

//Counter for Timer0: K = Freq * T
//I want 60 FPS -> T = 1 / (60) = 16 ms ca
//K = 16 * 10^-3 * 25 * 10^6 = 400000 -> 0x61A80
#define FPS_Time 0x4C4B40 //0x00061A80


//GAME MECHANICS INTERRUPTS PRIORITIES
/*
Pause button -> max pripority (0)
RIT -> priority 1
TIMER0 -> priority 2
TIMER1 (60 sec counter) -> priority 3
*/
#define RIT_Priority 0
#define TIM0_Priority 5


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
//direction: right
#define PLAYER_H PLAYER_CELLS_H * CELL_H
#define PLAYER_W PLAYER_CELLS_W * CELL_W
static uint8_t player[PLAYER_H][PLAYER_W] = {
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1}, // Riga 1
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 2
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 0}, // Riga 3
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, // Riga 4
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // Riga 5
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // Riga 6
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, // Riga 7
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 0}, // Riga 8
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 9
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1} // Riga 10
};


//GFX
void placePills4();
void drawPills4();
void drawPills();
void drawWalls(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height);
void drawDoor(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height);
void drawTunnel(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height);
void drawBlanks();
uint16_t getPixelX(uint16_t cellX); 
uint16_t getPixelY(uint16_t cellY); 
void drawPlayer(uint16_t cellX, uint16_t cellY, uint8_t direction);
void drawPlayerByPixels(uint16_t pixelX, uint16_t pixelY, uint8_t direction);
void erasePlayer(uint16_t cellX, uint16_t cellY);
#endif