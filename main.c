/*
 * tetris_3.c
 *
 * Created: 3/1/2019 11:43:55 AM
 * Author : jackson lu
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "nokia5110.h"
#include "nokia5110.c"
#include "grid.h"
#include "grid.c"
#include "block_line.h"
#include "block_line.c"
#include <time.h>

//==============================================================//
unsigned char start_endgame;
unsigned char end_endgame;
unsigned char complete_mount;
unsigned char break_me;
unsigned char check_full;
unsigned long DROP_elapsedTime;
unsigned char current_score;

unsigned char tick_limit;
unsigned char t = 0;
unsigned char finished_block;
uint8_t holy_random;

struct Grid new_grid;

struct Block_line temp_block;

//struct Block_line first_block;
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;				// Start count from here , down to 0.
unsigned long _avr_timer_cntcurr = 0;
unsigned long _case = 0;
int32_t seed_time = 0;
//----------------------------------------------------Timer
void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	seed_time++;
	SREG |= 0x80;
									// 00001000
}
void TimerOff(){
	TCCR1B = 0x00;
}
void TimerISR(){
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
//----------------------------------------------------Timer----
enum Start_handler{_wait,_start} state_1;
void Start_Handler_Tick(){
	
	
	//Transitions
	switch(state_1){
		case _wait:
			if((PINA & ( 1 << 4)) == 0){
				grid_init();
				state_1 = _start;
				start_endgame = 1;
				break;
			}else{
				state_1 = _wait;
				break;
			}
		case _start:
			//state_1 = _wait;
			if((PINA & ( 1 << 4)) == 0){
				state_1 = _wait;
				break;
			}else{
				state_1  = _start;
				break;
			}
		default:
			state_1 = _wait;
			break;
	}	
	
	//State Actions
	switch(state_1){
		case _wait:
			new_grid._count = 0;
			new_grid._timer = 4000;
			new_grid._score = 0;
			current_score = 0;
			start_endgame = 0;
			break;
		case _start:
			//start_endgame = 1;
			break;
		default:
			break;
	}
};

enum Game_ON{_wait1,_start1,show_score} state_2;
void Game_ON_Tick(){
	unsigned char c;
	switch(state_2){
		case _wait1:
			if(!start_endgame){
				state_2 = _wait1;
				break;
			}else{
				state_2 = _start1;
				break;
			}
		case _start1:
			if(start_endgame && !end_endgame){
				state_2 = _start1;
				break;
				
// 				if(new_grid._count <= new_grid._timer){
// 					state_2 = _start1;
// 					break;
// 				}else{
// 					state_2 = show_score;
// 					end_endgame = 1;
// 					break;
// 				}
			}else if(start_endgame && end_endgame){
				state_2 = show_score;
				break;
			}else if(!start_endgame && !end_endgame){
				state_2 = _wait1;
				break;
			}
		case show_score:
			if(c <= 5){
				PORTD = current_score;
				state_2 = show_score;
				c++;
				break;
			}else{
				state_2 = _wait1;
				end_endgame = 0;
				break;
			}
		default:
			state_2 = _wait1;
			break;
	}	
	switch(state_2){
		case _wait1:
			end_endgame = 0;
			c = 0;
			break;
		case _start1:
			//struct Block_line temp_block;
			//Creating a random block and only initialize it.. 
			if(new_grid._count == 0){;
				
				//Initializing a random block 
				holy_random = random_number();
				temp_block.coord_x = 62;
				temp_block.coord_y = 40;
				temp_block.object_type = holy_random;
				temp_block.finished_line = 0;

				draw_me(&temp_block);
			}else{
				
				//If the count is not fresh, check if the temp block has been dropped. 
				if(has_dropped(&temp_block) == 0){
					
					//Keep dropping it. 
					keep_dropping_me(&temp_block);
					//grid_render();
					

				}else{
					holy_random = random_number();
					temp_block.coord_x = 62;
					temp_block.coord_y = 40;
					temp_block.finished_line = 0;
					temp_block.object_type = holy_random;
					
					if(!nokia_lcd_check_pixel(62,40)){
						nokia_lcd_clear();
						nokia_lcd_write_string("You Lose",1);
						start_endgame = 0;
					}else{
						draw_me(&temp_block);
					}
					//draw_me(&temp_block);
				
					
					
					
					
					

				}
			}
			
			new_grid._count++;
			break;
			
		case show_score:
			break;
			//Show the score on the grid. 
		default:
			break;
	}
};

enum Rotate{_wait2,rotate_left,rotate_right} state_3;
void Rotate_tick(struct Block_line *tmp_blk){
	unsigned char left_btn2 = 0x00;
	unsigned char right_btn2 = 0x00;
	left_btn2 = ~PINA & 0x08;
	right_btn2 = ~PINA & 0x04;

	switch(state_3){
		case _wait2:
			if((left_btn2 && right_btn2) && (start_endgame)){
				state_3 = _wait2;

			}else if((left_btn2 && !right_btn2) && (start_endgame)){
				state_3 = rotate_left;

			}else if((right_btn2 && !left_btn2) && (start_endgame)){
				state_3 = rotate_right;

			}
			break;
		case rotate_left:
			state_3 = _wait2;
			break;
		case rotate_right:
			state_3 = _wait2;
			break;
		default:
			state_3 = _wait2;
			break;
	}	
	switch(state_3){
		case _wait2:
			break;
		
		case rotate_left:
		
		
			rotate_lefts(tmp_blk);
			//grid_render();
			break;
			
			//Rotate the tetris block CCW
		case rotate_right:
		
		
			rotate_rights(tmp_blk);
			//grid_render();
			break;
			//Rotate the tetris block CW
		default:
			break;
	}
};

enum movement{_wait3,move_left,move_right} state_4;
void movement_tick(struct Block_line *temp_blk){
	unsigned char left_btn = 0x00;
	unsigned char right_btn = 0x00;
	
	//PA3 - left | PA4 - right
	
	right_btn = ~PINA & 0x01;
	left_btn = ~PINA & 0x02;
	switch(state_4){
		case _wait3:
			if((left_btn && right_btn) && (start_endgame)){
				state_4 = _wait3;
				
			}else if((!right_btn && left_btn) && (start_endgame)){
				state_4 = move_left;
				
			}else if((!left_btn && right_btn) && (start_endgame)){
				state_4 = move_right;
				
			}
			break;
		case move_left:
			state_4 = _wait3;
			break;
		case move_right:
			state_4 = _wait3;
			break;
		default:
			state_4 = _wait3;
			break;
	}	
	
	switch(state_4){
		case _wait3:
			break;
		case move_left:
		
			move_lefts(temp_blk);
			//grid_render();
			break;
			//move left
			
		case move_right:
			move_rights(temp_blk);
			//grid_render();
			break;
			//move right 
		default:
			break;
	}
};

enum tetris_attach{_wait4,_enter} state_5;
void tetris_attach_tick(){
	unsigned char drop_btn = ~PINA & 0x32;
	
	switch(state_5){
		case _wait4:
			if((PINA & ( 1 << 5)) == 0){
				state_5 = _enter;
				break;
			}else{
				state_5 = _wait4;
				break;
			}
		case _enter:
			state_5 = _wait4;
			break;
		default:
			state_5 = _wait4;
			break;
	}	
	switch(state_5){
		case _wait4:
			complete_mount = 0;
			break;
		case _enter:
			drop_me(&temp_block);
			//Get the current tetris block x,y coordinates and drop that motherfucker//
			//find the y distance from the pixel that is not 0.
			//attach it to that pixel and draw it out. 
			complete_mount = 1;
			break;
		default:
			break;
	}
	
};
enum check_line{_wait5,check_line_complete} state_6;
void check_line_tick(){
	
	switch(state_6){
		case _wait5:
			if(complete_mount){
				state_6 = check_line_complete;
				state_6 = _wait5;
				break;
			}else{
				state_6 = _wait5;
				break;
			}
		case check_line_complete:
			state_6 = _wait5;
			break;
		default:
			state_6 = _wait5;
			break;
	}	
	
	switch(state_6){
		case _wait5:
			break_me = 0;
			check_full = 0;
		case check_line_complete:
			destroy_line(&temp_block);
			//Get the corresponding last line and check if all the pixels are filled.
			//If its not full
			if(temp_block.finished_line == 1){
				current_score++;
				temp_block.finished_line = 0;
			}
				check_full = 1;
			//If its full
			break;
		default:
			break;
	}
};



int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	//PINA = 0xFF;
	
	unsigned long Start_elapsedTime = 250;
	unsigned long GameON_elapsedTime = 500;
	unsigned long movement_elapsedTime = 250;
	unsigned long rotate_elapsedTime = 250;
	unsigned long attach_elapsedTime = 250;
	unsigned long check_elapsedTime = 250;
	
	const unsigned long timerPeriod = 250;
	
	TimerSet(timerPeriod);
	TimerOn();
	srand(seed_time);
	
	state_4 = _wait3;
	state_3 = _wait2;
	state_1 = _wait;
	state_2 = _wait1;
	state_5 = _wait4;
	state_6 = _wait5;

	
	while (1)
	{	
		if(Start_elapsedTime >= 250){
			Start_Handler_Tick();
			Start_elapsedTime = 0;
		}
		
		
		if(GameON_elapsedTime >= 500){
			Game_ON_Tick();
			GameON_elapsedTime = 0;
		}
		
		if(movement_elapsedTime >= 250){
			movement_tick(&temp_block);
			movement_elapsedTime = 0;
		}
		
		if(rotate_elapsedTime >= 250){
			Rotate_tick(&temp_block);
			rotate_elapsedTime = 0;
		}
		
		if(attach_elapsedTime >= 250){
			tetris_attach_tick();
			attach_elapsedTime = 0;
		}
		if(check_elapsedTime >= 250){
			check_line_tick();
			check_elapsedTime = 0;
		}

		
		//Instantly 
		grid_render();
		PORTD = current_score; 
		
		while(!TimerFlag){};
		TimerFlag = 0;
		
		Start_elapsedTime += timerPeriod;
		GameON_elapsedTime += timerPeriod;
		movement_elapsedTime += timerPeriod;
		rotate_elapsedTime += timerPeriod;
		attach_elapsedTime += timerPeriod;
		check_elapsedTime += timerPeriod;
		
		
	}
	
}


