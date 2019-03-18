#include <avr/io.h>
#include "block_line.h"
#include "nokia5110.h"
#include <stdbool.h>
uint8_t random_number(){
	uint8_t _random = rand() % 13 + 1;
	return _random;
}
uint8_t all_fail(){
	//1 means true 
	uint8_t full_line = 0;
	for(uint8_t i = 44 ; i < 80 ;i++){
		if(!nokia_lcd_check_pixel(i,40)){
			full_line = 1;
			break;
		}
	}
	return full_line;
}
void destroy_line(struct Block_line *holy_field){
	uint8_t start_x = 80;
	uint8_t start_y = 3;
	uint8_t _passed = 1;
	uint8_t _counter = 0;

	for(uint8_t i = 0 ; i < 36;i++){
		if(!nokia_lcd_check_pixel(start_x-i,start_y)){
			_passed = 0;
			_counter++;
		}
	}
	
	//It has been erased 
	if(_counter >= 36){
		nokia_lcd_clear_rectangle(80,4,36,3);
		grid_render();
		drop_down();
		holy_field->finished_line = 1;
	}
}
//Top x : 80 , Top y: 43 : End x : 44: , End y : 43
//Top x : 80 , Top y: 4  , End x : 44  , End y : 4
void refresh_boiz(){
	//Drop down the pixels
	for(int i = 80 ; i > 44;i--){
		for(int j = 43 ; j > 3; j--){
			if(!nokia_lcd_check_pixel(i,j)){
				nokia_landed_set_pixel(i,j,1);
			}
		}
	}
	//nokia_lcd_clear();
	
	
	//nokia_lcd_set_cursor(0, 0);
	
	//Drawing the grid of the left side
	for(int i = 0 ; i <= 45;i++){
		nokia_landed_set_pixel(83,i,1);
	}
	
	//Drawing the grid that is inside the grid left side
	for(int i = 1; i <= 44; i++){
		nokia_landed_set_pixel(82,i,1);
	}
	
	//Drawing a grid that is on the top
	for(int j = 83;j >= 0;j--){
		nokia_landed_set_pixel(j,45,1);
	}
	
	//Drawing a grid thats inside the grid on the grid top
	for(int j = 82; j >= 42;j--){
		nokia_landed_set_pixel(j,44,1);
	}
	
	//Drawing a grid on the right side
	for(int x = 0; x < 46;x++){
		nokia_landed_set_pixel(41,x,1);
	}
	
	//Drawing a grid thats inside a grid on the right side
	for(int x = 1; x < 45;x++){
		nokia_landed_set_pixel(42,x,1);
	}
	
	
	//Drawing inside the grid of the bottom
	for(int f = 83; f >= 0 ;f--){
		nokia_landed_set_pixel(f,0,1);
	}
	
	//Drawing inside of the grid that is the bottom
	for(int f = 82; f >= 42;f--){
		nokia_landed_set_pixel(f,1,1);
	}
	nokia_lcd_clear();
	nokia_landed_lcd_render2();
	nokia_landed_clear();
	grid_render();
	//Copy it to the nokia lcd second array
	//Draw the grid
	//Show it.
}
void drop_down(){
	//Drop down the pixels
	for(int i = 80 ; i > 44;i--){
		for(int j = 43 ; j > 4;j--){
			if(!nokia_lcd_check_pixel(i,j)){
				nokia_landed_set_pixel(i,j-3,1);
			}
		}
	}
	//nokia_lcd_clear();
	
	
	//nokia_lcd_set_cursor(0, 0);
	
	//Drawing the grid of the left side
	for(int i = 0 ; i <= 45;i++){
		nokia_landed_set_pixel(83,i,1);
	}
	
	//Drawing the grid that is inside the grid left side
	for(int i = 1; i <= 44; i++){
		nokia_landed_set_pixel(82,i,1);
	}
	
	//Drawing a grid that is on the top
	for(int j = 83;j >= 0;j--){
		nokia_landed_set_pixel(j,45,1);
	}
	
	//Drawing a grid thats inside the grid on the grid top
	for(int j = 82; j >= 42;j--){
		nokia_landed_set_pixel(j,44,1);
	}
	
	//Drawing a grid on the right side
	for(int x = 0; x < 46;x++){
		nokia_landed_set_pixel(41,x,1);
	}
	
	//Drawing a grid thats inside a grid on the right side
	for(int x = 1; x < 45;x++){
		nokia_landed_set_pixel(42,x,1);
	}
	
	
	//Drawing inside the grid of the bottom
	for(int f = 83; f >= 0 ;f--){
		nokia_landed_set_pixel(f,0,1);
	}
	
	//Drawing inside of the grid that is the bottom
	for(int f = 82; f >= 42;f--){
		nokia_landed_set_pixel(f,1,1);
	}
	nokia_lcd_clear();
	nokia_landed_lcd_render2();
	nokia_landed_clear();
	grid_render();
	//Copy it to the nokia lcd second array
	//Draw the grid 
	//Show it. 
}
uint8_t check_back(uint8_t _front , uint8_t _back){
	if(!nokia_lcd_check_pixel(_front-3,_back)){
		return 0;
	}
	return 1;
}
void drop_me(struct Block_line *tmp_blk){
	uint8_t _x = tmp_blk->coord_x;
	uint8_t _y = tmp_blk->coord_y;
	//Counter of how many times this check pixel worked..
	//Use that counter and multiple it by 3.. add that to the y value. 
	//Clear the rectangle and just set it there. 
	switch(tmp_blk->object_type){
		case 1: 
			_x -= 1;
			_y -= 15;
			
			break;
		case 2:
			_x -= 1;
			_y -= 4;
			break;
		case 3:
			_x -= 1;
			_y -= 4;
			break;
		case 4:
			_x -= 4;
			_y -= 10;
			break;
		case 5:
			_x -= 1;
			_y -= 7;
			break;
		case 6:
			_x -= 1;
			_y -= 7;
			break;
		case 7:
			_x -= 1;
			_y -= 10;
			break;
		case 8:
			_x -= 4;
			_y -= 7;
			break;
		case 9:
			_x -= 1;
			_y -= 10;
			break;
		case 10:
			_x -= 1;
			_y -= 7;
			break;
		case 11:
			_x += 2;
			_y -= 10;
			break;
		case 12:
			_x -= 4;
			_y -= 7;
			break;
		default:
			break;
	}
	uint8_t _counter = 0;
	while(nokia_lcd_check_pixel(_x,_y)){
		_counter++;
		_y -= 3;
	}
	uint8_t final_destination = _counter * 3;
	
	switch(tmp_blk->object_type){
		case 1:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,15);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 2:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 3:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 4:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,6);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 5:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,6,3);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 6:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 7:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,3);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 8:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,9,3);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 9:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 10:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,3,6);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 11:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,6);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		case 12:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,6,3);
			tmp_blk->coord_y -= final_destination;
			draw_me(tmp_blk);
			break;
		default:
			break;
	}
}
	


uint8_t landed_on_someone(struct Block_line *tmp_blk){
	
	}
uint8_t has_dropped(struct Block_line *tmp_blk){
	
	uint8_t finished_dropping = 0;
	switch(tmp_blk->object_type){
		
		case 1:
			if((tmp_blk->coord_y - 18 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			
			break;
		case 2:
			if((tmp_blk->coord_y - 6 <= 0) || (!check_touch(tmp_blk))){
				finished_dropping = 1;
			}
			break;
		case 3:
			if((tmp_blk->coord_y - 6 <= 0) || (!check_touch(tmp_blk))){
				finished_dropping = 1;
			}
			break;
		case 4:
			if((tmp_blk->coord_y - 12 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			break;
		case 5:
			if((tmp_blk->coord_y - 9 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			break;
		case 6:
			if((tmp_blk->coord_y - 9 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			break;
		case 7:
			if((tmp_blk->coord_y - 12 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			break;
		case 8:
			if((tmp_blk->coord_y - 9 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			break;
		case 9:
			if((tmp_blk->coord_y - 12 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			break;
		case 10:
			if((tmp_blk->coord_y - 9 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			break;
		case 11:
			if((tmp_blk->coord_y - 12 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			break;
		case 12:
			if((tmp_blk->coord_y - 9 <= 0) || (check_touch(tmp_blk) == 0)){
				finished_dropping = 1;
			}
			break;
		default:
			break;
		
	}
	return finished_dropping;
}
	//If the block while it has been dropping that had touch a subject. 
uint8_t check_touch(struct Block_line *tmp_blk){
	uint8_t _passed = 1; 
	switch(tmp_blk->object_type){
		case 1:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y - 17)){
				_passed = 0;
			}
			break;
		case 2:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y-5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-5,tmp_blk->coord_y-5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-8,tmp_blk->coord_y-5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-11,tmp_blk->coord_y-5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-14,tmp_blk->coord_y-5)) {
				_passed = 0;
			}
			break;
			
		case 3:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y-5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-5,tmp_blk->coord_y-5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-8,tmp_blk->coord_y-5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-11,tmp_blk->coord_y-5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-14,tmp_blk->coord_y-5)) {
				_passed = 0;
			}
			break;
		case 4:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y -8)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-5,tmp_blk->coord_y -11)){
				_passed = 0;
			}
			break;
		
		case 5:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y -8)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x+2,tmp_blk->coord_y -8)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-4,tmp_blk->coord_y -5)){
				_passed = 0;
			}
			break;

		case 6:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y -8)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x+2,tmp_blk->coord_y -8)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-5,tmp_blk->coord_y -8)){
				_passed = 0;
			}
			break;

		case 7:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y -11)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x+2,tmp_blk->coord_y -8)){
				_passed = 0;
			}
			break;
		case 8:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-5,tmp_blk->coord_y -8)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y -5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-8,tmp_blk->coord_y -5)){
				_passed = 0;
			}
			break;
			
		case 9:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y -11)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-5,tmp_blk->coord_y -8)){
				_passed = 0;
			}
			break;

		case 10:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y -8)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-5,tmp_blk->coord_y -8)){
				_passed = 0;
			}
			break;
		case 11:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-1,tmp_blk->coord_y -8)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x+2,tmp_blk->coord_y -11)){
				_passed = 0;
			}
			break;
		case 12:
			if(!nokia_lcd_check_pixel(tmp_blk->coord_x-2,tmp_blk->coord_y -5)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y -8)){
				_passed = 0;
			}else if(!nokia_lcd_check_pixel(tmp_blk->coord_x-8,tmp_blk->coord_y -8)){
				_passed = 0;
			}
			break;
		default:
			break;
		
	}
	return _passed; 
	}
void keep_dropping_me(struct Block_line *tmp_blk){
	switch(tmp_blk->object_type){
		case 1:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,15);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 2:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 3:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 4:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,6);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 5:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,6,3);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 6:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 7:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,3);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 8:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,9,3);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 9:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 10:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,3,6);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 11:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,6);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		case 12:
			nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
			nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,6,3);
			tmp_blk->coord_y -= 3;
			draw_me(tmp_blk);
			break;
		default:
			break;
	}
	
}


void draw_me(struct Block_line *tmp_blk){
	switch(tmp_blk->object_type){
		case 1:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,15);
			break;
		case 2:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
			break;
		case 3:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
			break;
		case 4:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,6);
			break;
		case 5:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
			draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,6,3);
			break;
		case 6:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,3);
			draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
			break;
		case 7:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
			draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,3);
			break;
		case 8:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,9,3);
			draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
			break;
		case 9:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
			draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
			break;
		case 10:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,6);
			break;
		case 11:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
			draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,6);
			break;
		case 12:
			draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
			draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,6,3);
			break;
		default:
			break;
	}
	}
void rotate_lefts(struct Block_line *tmp_blk){
	uint8_t check_me = 1;
	switch(tmp_blk->object_type){
		case 1:
			
			check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,1,2); 
			if(tmp_blk->coord_x + 12 >= 82){
				break;
			}
			if(check_me == 1){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,15);
				draw_random_rectangle(tmp_blk->coord_x + 12,tmp_blk->coord_y - 12,15,3);
				tmp_blk->coord_x = tmp_blk->coord_x + 12;
				tmp_blk->coord_y = tmp_blk->coord_y - 12;
				tmp_blk->object_type = 2;
			}
			break;
		case 2:
		
			check_me = nokia_lcd_check_hella_pixel( tmp_blk->coord_x,tmp_blk->coord_y,3,1);
			if(check_me == 1){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y+12,3,15);
				tmp_blk->object_type = 1;
				tmp_blk->coord_y = tmp_blk->coord_y + 12;
			}
			break;

		case 3:
			check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,3,1);
			if(check_me == 1){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y+12,3,15);
				tmp_blk->object_type = 1;
				tmp_blk->coord_y = tmp_blk->coord_y + 12;
			}
			break;
			
		case 4:
			check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,4,5);
			if(tmp_blk->coord_x + 3 >= 82){
				break;
			}
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x -3,tmp_blk->coord_y-3,3,6);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,6,3);
				tmp_blk->object_type = 5;
			}
			break;
		case 5:
			break;
		case 6:
			check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,6,7);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,3);
				tmp_blk->object_type = 7;
			}
			break;
		case 7: 
			if(tmp_blk->coord_x - 6 <= 42){
				break;
			}
			check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,7,8);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y -3,3,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-6,3,3);
				draw_random_rectangle(tmp_blk->coord_x +3 , tmp_blk->coord_y-3,9,3);
				tmp_blk->object_type = 8;
				tmp_blk->coord_x = tmp_blk->coord_x + 3;
				tmp_blk->coord_y = tmp_blk->coord_y - 3;
			}
			break;
		case 8:
			check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,8,9);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,9,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y+3,3,9);
				draw_random_rectangle(tmp_blk->coord_x-6,tmp_blk->coord_y,3,3);
				tmp_blk->object_type = 9;
				tmp_blk->coord_x = tmp_blk->coord_x - 3;
				tmp_blk->coord_y = tmp_blk->coord_y + 3;
			}
			break;
		case 9:
			if(tmp_blk->coord_x + 3 >= 82){
				break;
			}
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,9,6);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
				tmp_blk->object_type = 6;

			}
			break;
		case 10:
		case 11:
			if(tmp_blk->coord_x - 3 <= 42){
				break;
			}
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,11,12);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,6);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,6,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,6,3);
				tmp_blk->object_type = 12;
				tmp_blk->coord_x = tmp_blk->coord_x + 3;
			}
			break;
		case 12:
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,12,11);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,6,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y+3,3,6);
				tmp_blk->object_type = 11;
				tmp_blk->coord_x = tmp_blk->coord_x - 3;
				tmp_blk->coord_y = tmp_blk->coord_y + 3;
			}
			break;
		default:
			break;
	}
}
	void rotate_rights(struct Block_line *tmp_blk){
	uint8_t check_me = 1;
	switch(tmp_blk->object_type){
		case 1:
			
			check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,1,3);
			if(tmp_blk->coord_x - 15 <= 42){
				break;
			}
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,15);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-12,15,3);
				tmp_blk->object_type = 3;
				tmp_blk->coord_y = tmp_blk->coord_y - 12;
			}
			break;
		case 2:
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,2,1);
				if(tmp_blk->coord_y + 12 >= 44){
					break;
				}
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
				draw_random_rectangle(tmp_blk->coord_x-12,tmp_blk->coord_y+12,3,15);
				tmp_blk->object_type = 1;
				tmp_blk->coord_x = tmp_blk->coord_x - 12;
				tmp_blk->coord_y = tmp_blk->coord_y + 12;
			}
			break;
		case 3:
			break;
		case 4:
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,4,5);
			if(tmp_blk->coord_x + 3 >= 82){
				break;
			}
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,6);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,6,3);
				tmp_blk->object_type = 5;
			}
			break;
		case 5:
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,5,4);
				if(tmp_blk->coord_x + 3 >= 82){
					break;
				}
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,6,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,3,6);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,3,6);
				tmp_blk->object_type = 4;
				tmp_blk->coord_x = tmp_blk->coord_x + 3;
			}
			break;
		case 6:
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,6,9);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
				tmp_blk->object_type = 9;
			}
			break;
		case 7:
			if(tmp_blk->coord_x - 6 <= 42){
				break;
			}
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,7,6);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
				tmp_blk->object_type = 6;
			}
			break;
		case 8:
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,8,7);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,9,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y+3,3,9);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,3);
				tmp_blk->coord_x = tmp_blk->coord_x - 3;
				tmp_blk->coord_y = tmp_blk->coord_y + 3;
				tmp_blk->object_type = 7;
			}
			break;
		case 9:
				
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,9,8);
				if(tmp_blk->coord_x + 3 >= 82){
					break;
				}
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-6,3,3);
				tmp_blk->coord_x = tmp_blk->coord_x + 3;
				tmp_blk->coord_y = tmp_blk->coord_y - 3;
				tmp_blk->object_type = 8;
			}
			break;
		case 10:
		case 11:
			if(tmp_blk->coord_x - 6 <= 42){
				break;
			}
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,11,12);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,6);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,6,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,6,3);
				tmp_blk->object_type = 12;
				tmp_blk->coord_x = tmp_blk->coord_x + 3;
			}
			break;
		case 12:
				check_me = nokia_lcd_check_hella_pixel(tmp_blk->coord_x,tmp_blk->coord_y,12,11);
			if(check_me){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,6,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y+3,3,6);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				tmp_blk->object_type = 11;
				tmp_blk->coord_x = tmp_blk->coord_x - 3;
				tmp_blk->coord_y = tmp_blk->coord_y + 3;
			}
			break;
		default:
			break;
	}
	}
void move_lefts(struct Block_line *tmp_blk){
	uint8_t _passed = 1;
	switch(tmp_blk->object_type){
		case 1:
			//Top block
			if(tmp_blk->coord_x+3 >= 83){
				_passed = 0;
				break;
			}
			
		
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y-6);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y-9);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y-12);
			
			if(_passed == 1){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,15);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,3,15);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		case 2:
			
			if(tmp_blk->coord_x+3 >= 83){
				_passed = 0;
				break;
			}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,15,3);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			
			break;
		case 3:
		
				if(tmp_blk->coord_x+3 >= 83){
					_passed = 0;
					break;
				}
			 
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,15,3);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		case 4:
		
				if(tmp_blk->coord_x+3 >= 83){
					_passed = 0;
					break;
				}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+6,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+6,tmp_blk->coord_y-6);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,6);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,3,6);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,3,6);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
				
			}
			break;
		case 5:
			 
				if(tmp_blk->coord_x+6 >= 83){
					_passed = 0;
					break;
				}
			 
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+6,tmp_blk->coord_y-3);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,6,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,6,3);
				draw_random_rectangle(tmp_blk->coord_x+6,tmp_blk->coord_y-3,6,3);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		case 6:
			 
				if(tmp_blk->coord_x+6 >= 83){
					_passed = 0;
					break;
				}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+6,tmp_blk->coord_y-3);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,3,3);
				draw_random_rectangle(tmp_blk->coord_x+6,tmp_blk->coord_y-3,9,3);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		case 7:
			 
				if(tmp_blk->coord_x+6 >= 83){
					_passed = 0;
					break;
				}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+6,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y-6);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,3,9);
				draw_random_rectangle(tmp_blk->coord_x+6,tmp_blk->coord_y-3,3,3);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		case 8:
			 
				if(tmp_blk->coord_x+3 >= 83){
					_passed = 0;
					break;
				}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x,tmp_blk->coord_y-3);

			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,9,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,9,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,3,3);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		case 9:
			 
				if(tmp_blk->coord_x+3 >= 83){
					_passed = 0;
					break;
				}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y-6);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,3,9);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,3,3);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		case 10:
			 
				if(tmp_blk->coord_x+3 >= 83){
					_passed = 0;
					break;
				}
			 
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y-3);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,6);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,6,6);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		case 11:
		
			if(tmp_blk->coord_x+6 >= 83){
					_passed = 0;
					break;
				}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+6,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+6,tmp_blk->coord_y-6);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,6);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,3,6);
				draw_random_rectangle(tmp_blk->coord_x+6,tmp_blk->coord_y-3,3,6);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		case 12:
			 
				if(tmp_blk->coord_x+3 >= 83){
					_passed = 0;
					break;
				}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x+3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x,tmp_blk->coord_y-3);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,6,3);
				draw_random_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y,6,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,6,3);
				tmp_blk->coord_x = tmp_blk->coord_x+3;
			}
			break;
		default:
			break;
	}
	}
void move_rights(struct Block_line *tmp_blk){
	uint8_t _passed = 1;
	switch(tmp_blk->object_type){
		case 1:
		
			if(tmp_blk->coord_x-6 < 42){
				_passed = 0;
				break;
			}
			//Top block
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y-6);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y-9);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y-12);
			
			if(_passed == 1){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,15);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,3,15);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		case 2:
			
			if(tmp_blk->coord_x-18 <= 42){
				_passed = 0;
				break;
			}
			
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-15,tmp_blk->coord_y);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,15,3);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			
			break;
		case 3:
			
			if(tmp_blk->coord_x-18 <= 42){
				_passed = 0;
				break;
			}
			
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-15,tmp_blk->coord_y);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,15,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,15,3);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		case 4:
			
			if(tmp_blk->coord_x-9 < 42){
				_passed = 0;
				break;
			}
			
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y-6);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,6);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,3,6);
				draw_random_rectangle(tmp_blk->coord_x-6,tmp_blk->coord_y-3,3,6);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
				
			}
			break;
		case 5:
		
			if(tmp_blk->coord_x-9 < 42){
				_passed = 0;
				break;
			}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,6,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,6,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,6,3);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		case 6:
			if(tmp_blk->coord_x-9 < 42){
				_passed = 0;
				break;
			}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y-3);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,9,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,3,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,9,3);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		case 7:
			if(tmp_blk->coord_x-6 < 42){
				_passed = 0;
				break;
			}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y-6);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,3,9);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		case 8:
			if(tmp_blk->coord_x-12 < 42){
				_passed = 0;
				break;
			}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-9,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y-3);

			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,9,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,9,3);
				draw_random_rectangle(tmp_blk->coord_x-6,tmp_blk->coord_y-3,3,3);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		case 9:
			if(tmp_blk->coord_x-9 < 42){
				_passed = 0;
				break;
			}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y-6);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,9);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,3,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,3,9);
				draw_random_rectangle(tmp_blk->coord_x-6,tmp_blk->coord_y-3,3,3);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		case 10:
		
			if(tmp_blk->coord_x-9 < 42){
				_passed = 0;
				break;
			}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y-3);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,6);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,6,6);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		case 11:
			
			if(tmp_blk->coord_x-6 < 42){
				_passed = 0;
				break;
			}
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-3,tmp_blk->coord_y-3);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x,tmp_blk->coord_y-6);
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,3,6);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x+3,tmp_blk->coord_y-3,3,6);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,3,6);
				draw_random_rectangle(tmp_blk->coord_x,tmp_blk->coord_y-3,3,6);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		case 12:
			
			if(tmp_blk->coord_x-12 < 42){
				_passed = 0;
				break;
			}
			
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-6,tmp_blk->coord_y);
			_passed = nokia_lcd_check_triple_pixel(tmp_blk->coord_x-9,tmp_blk->coord_y-3);
			
			
			if(_passed){
				nokia_lcd_clear_rectangle(tmp_blk->coord_x,tmp_blk->coord_y,6,3);
				nokia_lcd_clear_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y-3,6,3);
				draw_random_rectangle(tmp_blk->coord_x-3,tmp_blk->coord_y,6,3);
				draw_random_rectangle(tmp_blk->coord_x-6,tmp_blk->coord_y-3,6,3);
				tmp_blk->coord_x = tmp_blk->coord_x-3;
			}
			break;
		default:
			break;
	}
}