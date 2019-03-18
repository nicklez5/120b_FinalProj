#ifndef _GRID_H
#define _GRID_H
#include <avr/io.h>

#define MAX_X = 84
#define MAX_Y = 46

struct Grid{
	unsigned char _score;
	unsigned char _timer;
	unsigned char _count;
	unsigned char _current_row;
};
//Initializes the grid of the tetris game
void grid_init(void );
//Render the grid 
void grid_render(void);

//Show the score on the grid 
void grid_score_show(void);

//Draw the big ass midline on the grid 
void grid_draw_midline(void);

void draw_random_rectangle(uint8_t x,uint8_t y, uint8_t x_length ,uint8_t y_length);

#endif