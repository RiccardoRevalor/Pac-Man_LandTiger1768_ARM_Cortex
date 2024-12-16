#ifndef __GAME_H
#define __GAME_H

#include "../GLCD/GLCD.h"


//colors scheme
#define WALL_COLOR Blue
#define BACKGROUND_COLOR Black
#define STDPILL_COLOR Magenta
#define PWRPILL_COLOR Green
#define HOUSEDOOR_COLOR Red
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

//Standard pills dimensions in terms of pixels
//Each standard pill occupied 2x2 cells so 10x10 pixels
#define STDPILLS_CELLS_H 2
#define STDPILLS_CELLS_W 2



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
#define PWRPILL_CODE_1 3
#define PWRPILL_CODE_2 11
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
extern uint16_t plX;
extern uint16_t plY;


//IN-GAME TIMINGS
//60 seconds timer
//in the simulator approximately: 1 sec (real time) = 10 ms (simulator time) [ca]
//so set the time interval to 10 ms
//10 ms * 25c Mhz = 250000

/*
60 secondi
ho la timescale: Tempo base = 1 ms reale -> 0.00001024 s emulatore
1 s = 1000 ms
1000 / 1 = x = 0.00001024 -> x = 0.00001024 x 1000 = 0.01024 nell'emulatore


*/
#define TimeCounter_Time 0x3E800 //0x17D7840
//60 seconds time variable
extern uint16_t gameTime; //at first it's equal to 0 (defined in sample.c)
#define GAMETIME_LIMIT 60 //after that, the current game ends



//Counter for Timer0: K = Freq * T
//I want 60 FPS -> T = 1 / (60) = 16 ms ca
//K = 16 * 10^-3 * 25 * 10^6 = 400000 -> 0x61A80

//2 ms
//#define FPS_Time 0xC350 //0x4C4B40  //0x98968 //0x00061A80 //FOR SLOWER FPS: 0x4C4B40 
/*
Senza impostare il timescale sull'emulatore.
Voglio 30 fps = 1 / 32 ms circa
Tempo base = 1 ms reale -> 0.00001024 s emulatore
Faccio 32 x 0.00001024 s x 25 Mhz = 8192 -> 0x2000
*/
#define FPS_Time 0x2000
#define FPS_Time_DEBUG 0xBEBC20


//1 ms
//#define RIT_Time 0x186A0  //0x004C4B40
/*
Metto il contatore dimezzato rispetto a quello del timer0 per privilegiare gli input
16 x 0.00001024 x 25 Mhz = 4096 -> 0x1000
*/
#define RIT_Time 0x1000


//Timer to redraw GUI Texts
//3 ms
#define TextRedraw_Time 0x2000 //0x124F8


//GAME MECHANICS INTERRUPTS PRIORITIES
/*
Pause button -> max pripority (0)
RIT -> priority 1
TIMER0 -> priority 2
TIMER1 (60 sec counter) -> priority 3
*/
#define RIT_Priority 1
#define TIM0_Priority 2
#define TIM1_Priority 5
#define TIM2_Priority 2


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


extern unsigned int maze[YMAX][XMAX];


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

static uint8_t playerEating[PLAYER_H][PLAYER_W] = {
		{0, 0, 0, 1, 1, 1, 1, 0, 0, 0}, // Riga 1
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0}, // Riga 2
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 3
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Riga 4
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Riga 5
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Riga 6
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Riga 7
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 8
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0}, // Riga 9
    {0, 0, 0, 1, 1, 1, 1, 0, 0, 0} // Riga 10
};

#define STDPILLS_H STDPILLS_CELLS_H * CELL_H
#define STDPILLS_W STDPILLS_CELLS_W * CELL_W
static uint8_t stdPill[STDPILLS_H][STDPILLS_W] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Riga 1
    {0, 0, 0, 1, 1, 1, 1, 0, 0, 0}, // Riga 2
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0}, // Riga 3
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 4
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 5
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 6
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // Riga 7
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0}, // Riga 8
    {0, 0, 0, 1, 1, 1, 1, 0, 0, 0}, // Riga 9
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // Riga 10
};


extern uint8_t pwrPillsCounter;


//TUNNELS SPECIFICATIONS
#define LT_X 0
#define RT_X XMAX - 1
#define T_Y (YMAX - 2*MAZESTART) / 2 - 1
#define T_HEIGTH 8
#define T_WIDTH 1

//SCORE, LIVE
extern uint16_t score;
extern uint16_t life;

//GUI TEXTS
//SCORE POSITION
#define SCORE_X XMAX - 17
#define SCORE_Y 1
//60 SECONDS COUNTER
#define TIMECNT_X 1
#define TIMECNT_Y 1
//LIVES
#define LIFECNT_X 1
#define LIFECNT_Y YMAX - 4

//DEBUG MOVEMENTS
/*
0 -> DEBUG OFF
1 -> DEBUG ON
*/
#define DEBUG_MOVS 0

//GFX
void placePills4();
void drawPills4(uint16_t cellX, uint16_t cellY, uint8_t isPowerPill);
void drawPills();
void drawWalls(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height);
void drawMapWalls();
void drawDoor(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height);
void drawTunnel(uint16_t xS, uint16_t yS, uint16_t width, uint16_t height);
void drawBlanks();
void drawBlank(uint16_t xS, uint16_t yS);
uint16_t getPixelX(uint16_t cellX); 
uint16_t getPixelY(uint16_t cellY); 
void drawPlayer(uint16_t cellX, uint16_t cellY, uint8_t direction, uint8_t animation);
void drawPlayerByPixels(uint16_t pixelX, uint16_t pixelY, uint8_t direction);
void erasePlayer(uint16_t cellX, uint16_t cellY);
void erasePill(uint16_t cellX, uint16_t cellY);
void showGameOver();

static uint8_t gameOverGfxMap[YMAX][XMAX] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
#endif