#ifndef __GAME_H
#define __GAME_H

#include "../GLCD/GLCD.h"


//colors scheme
#define WALL_COLOR White

//player start position
#define START_XPOS 10
#define START_YPOS 15

//the screen size is 320x240. I divide these dimensions in cells so groups of pixels
//for now, cell size is 16x15 so we have 240/15 = 16 horizontal cells for each row, and 320/16 = 20 vertical cells for each column
//Real number of cells: 
#define YMAX 21
#define XMAX 16

#define CELL_H 16
#define CELL_W 15

//The labyrinth is mapped to a matrix of cells measuring 11x11

#define STD_PILLS 240
#define PWR_PILLS 6
/* integers for in-game objects:
		0 -> free cell (player can go through it)
		1 -> wall (player is blocked)
		2 -> standard pill
		3 -> power pill
		4 -> tele-transport tunnels (traversable by player to reach opposite parts of the map)
*/

/*
YMAX x XMAX = 21x16 = 336
240 pills -> 96 cells remaining
perimeter walls: 15+15+19+19: 67 -> 96 - 67 = 29
center walls = 7 -> 29 - 7 = 2
240 pills
6 power pills -> 22 - 6 = 18
*/
static unsigned int maze[YMAX][XMAX] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Riga superiore
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, 
    {4, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 4}, // Riga tunnel
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, 
    {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
		{1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Riga inferiore
};
		


//GFX
void placePills();
void drawPerimeterWalls();
#endif