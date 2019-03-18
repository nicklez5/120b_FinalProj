#include "nokia5110.h"
#include "grid.h"
#include <avr/io.h>


void grid_init(void ){
	
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);
	
	//Drawing the grid of the left side
	for(int i = 0 ; i <= 45;i++){
		nokia_lcd_set_pixel(83,i,1);
	}
	
	//Drawing the grid that is inside the grid left side
	for(int i = 1; i <= 44; i++){
		nokia_lcd_set_pixel(82,i,1);
	}
	
	//Drawing a grid that is on the top 
	for(int j = 83;j >= 0;j--){
		nokia_lcd_set_pixel(j,45,1);
	}
	
	//Drawing a grid thats inside the grid on the grid top
	for(int j = 82; j >= 42;j--){
		nokia_lcd_set_pixel(j,44,1);
	}
	
	//Drawing a grid on the right side 
	for(int x = 0; x < 46;x++){
		nokia_lcd_set_pixel(41,x,1);
	}
	
	//Drawing a grid thats inside a grid on the right side 
	for(int x = 1; x < 45;x++){
		nokia_lcd_set_pixel(42,x,1);
	}
	
	
	//Drawing inside the grid of the bottom 
	for(int f = 83; f >= 0 ;f--){
		nokia_lcd_set_pixel(f,0,1);
	}
	
	//Drawing inside of the grid that is the bottom
	for(int f = 82; f >= 42;f--){
		nokia_lcd_set_pixel(f,1,1);
	}
	
	
	
}
void draw_random_rectangle(uint8_t x,uint8_t y, uint8_t x_length ,uint8_t y_length){
	nokia_lcd_draw_rectangle(x,y,x_length,y_length);
}
void grid_render(void){
	nokia_lcd_render();
}
void grid_score_show(void){
	nokia_lcd_set_cursor(80,27);
	nokia_lcd_write_string("Score:",0);
	
	nokia_lcd_set_cursor(80,12);
	nokia_lcd_write_string("Time:",0);
}

void grid_draw_midline(void){
	
}