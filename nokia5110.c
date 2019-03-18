/* Nokia 5110 LCD AVR Library
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */

#include "nokia5110.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include "nokia5110_chars.h"
#include <stdbool.h>

static struct {
    /* screen byte massive */
    uint8_t screen[504];
	uint8_t landed[504];
    /* cursor position */
    uint8_t cursor_x;
    uint8_t cursor_y;

} nokia_lcd = {
    .cursor_x = 0,
    .cursor_y = 0
};

/**
 * Sending data to LCD
 * @bytes: data
 * @is_data: transfer mode: 1 - data; 0 - command;
 */
static void write(uint8_t bytes, uint8_t is_data)
{
	register uint8_t i;
	/* Enable controller */
	PORT_LCD &= ~(1 << LCD_SCE);

	/* We are sending data */
	if (is_data)
		PORT_LCD |= (1 << LCD_DC);
	/* We are sending commands */
	else
		PORT_LCD &= ~(1 << LCD_DC);

	/* Send bytes */
	for (i = 0; i < 8; i++) {
		/* Set data pin to byte state */
		if ((bytes >> (7-i)) & 0x01)
			PORT_LCD |= (1 << LCD_DIN);
		else
			PORT_LCD &= ~(1 << LCD_DIN);

		/* Blink clock */
		PORT_LCD |= (1 << LCD_CLK);
		PORT_LCD &= ~(1 << LCD_CLK);
	}

	/* Disable controller */
	PORT_LCD |= (1 << LCD_SCE);
}

static void write_cmd(uint8_t cmd)
{
	write(cmd, 0);
}

static void write_data(uint8_t data)
{
	write(data, 1);
}

/*
 * Public functions
 */

void nokia_lcd_init(void)
{
	register unsigned i;
	/* Set pins as output */
	DDR_LCD |= (1 << LCD_SCE);
	DDR_LCD |= (1 << LCD_RST);
	DDR_LCD |= (1 << LCD_DC);
	DDR_LCD |= (1 << LCD_DIN);
	DDR_LCD |= (1 << LCD_CLK);

	/* Reset display */
	PORT_LCD |= (1 << LCD_RST);
	PORT_LCD |= (1 << LCD_SCE);
	_delay_ms(10);
	PORT_LCD &= ~(1 << LCD_RST);
	_delay_ms(70);
	PORT_LCD |= (1 << LCD_RST);

	/*
	 * Initialize display
	 */
	/* Enable controller */
	PORT_LCD &= ~(1 << LCD_SCE);
	/* -LCD Extended Commands mode- */
	write_cmd(0x21);
	/* LCD bias mode 1:48 */
	write_cmd(0x13);
	/* Set temperature coefficient */
	write_cmd(0x06);
	/* Default VOP (3.06 + 66 * 0.06 = 7V) */
	write_cmd(0xC2);
	/* Standard Commands mode, powered down */
	write_cmd(0x20);
	/* LCD in normal mode */
	write_cmd(0x09);

	/* Clear LCD RAM */
	write_cmd(0x80);
	write_cmd(LCD_CONTRAST);
	for (i = 0; i < 504; i++)
		write_data(0x00);

	/* Activate LCD */
	write_cmd(0x08);
	write_cmd(0x0C);
}
void nokia_landed_clear(void){
	register unsigned i;
	/* Set column and row to 0 */
	write_cmd(0x80);
	write_cmd(0x40);
	/*Cursor too */
	nokia_lcd.cursor_x = 0;
	nokia_lcd.cursor_y = 0;
	/* Clear everything (504 bytes = 84cols * 48 rows / 8 bits) */
	for(i = 0;i < 504; i++)
		nokia_lcd.landed[i] = 0x00;
}
void nokia_lcd_clear(void)
{
	register unsigned i;
	/* Set column and row to 0 */
	write_cmd(0x80);
	write_cmd(0x40);
	/*Cursor too */
	nokia_lcd.cursor_x = 0;
	nokia_lcd.cursor_y = 0;
	/* Clear everything (504 bytes = 84cols * 48 rows / 8 bits) */
	for(i = 0;i < 504; i++)
		nokia_lcd.screen[i] = 0x00;
}

void nokia_lcd_power(uint8_t on)
{
	write_cmd(on ? 0x20 : 0x24);
}
void nokia_landed_lcd_render(void){
	register unsigned i;
	for(i = 0;i < 504;i++){
		nokia_lcd.landed[i] = nokia_lcd.screen[i];
	}
}

uint8_t nokia_lcd_check_pixel(uint8_t x, uint8_t y){
	uint8_t *byte = &nokia_lcd.screen[y/8*84+x];
	if(*byte & (1 << (y%8)) ) {
		return 0;
	}
	return 1;
}
void nokia_landed_set_pixel(uint8_t x , uint8_t y, uint8_t value){
	uint8_t *byte = &nokia_lcd.landed[y/8*84+x];
	if(value)
		*byte |= (1 << (y % 8));
	else
		*byte &= ~(1 << (y & 8));
}
void nokia_lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value)
{
	uint8_t *byte = &nokia_lcd.screen[y/8*84+x];
	if (value)
		*byte |= (1 << (y % 8));
	else
		*byte &= ~(1 << (y %8 ));
}

void nokia_lcd_write_char(char code, uint8_t scale)
{
	register uint8_t x, y;

	for (x = 0; x < 5*scale; x++)
		for (y = 0; y < 7*scale; y++)
			if (pgm_read_byte(&CHARSET[code-32][x/scale]) & (1 << y/scale))
				nokia_lcd_set_pixel(nokia_lcd.cursor_x + x, nokia_lcd.cursor_y + y, 1);
			else
				nokia_lcd_set_pixel(nokia_lcd.cursor_x + x, nokia_lcd.cursor_y + y, 0);

	nokia_lcd.cursor_x += 5*scale + 1;
	if (nokia_lcd.cursor_x >= 84) {
		nokia_lcd.cursor_x = 0;
		nokia_lcd.cursor_y += 7*scale + 1;
	}
	if (nokia_lcd.cursor_y >= 48) {
		nokia_lcd.cursor_x = 0;
		nokia_lcd.cursor_y = 0;
	}
}

void nokia_lcd_write_string(const char *str, uint8_t scale)
{
	while(*str)
		nokia_lcd_write_char(*str++, scale);
}

void nokia_lcd_set_cursor(uint8_t x, uint8_t y)
{
	nokia_lcd.cursor_x = x;
	nokia_lcd.cursor_y = y;
}
//Returns 0 if theres a inside
//Returns 1 if theres nothing 
uint8_t nokia_lcd_check_triple_pixel(uint8_t x, uint8_t y){
	uint8_t final_value = 1;
	for(uint8_t i = 0; i < 2;i++){
		for(uint8_t j = 0; j < 2;j++){
			if(nokia_lcd_check_pixel(x-i,y-j-1) == 0){
				final_value = 0;
			}
		}
	}
	return final_value;
}
void nokia_lcd_clear_rectangle(uint8_t x,uint8_t y ,uint8_t x_length ,uint8_t y_length){
		//Draw top line
		for(uint8_t i = 0; i < x_length ; i++){
			nokia_lcd_set_pixel(x-i,y,0);
		}
		
		//Draw the left line of the rectangle
		for(uint8_t j = 1 ;j < y_length ; j++){
			nokia_lcd_set_pixel(x,y-j,0);
		}
		
		//Draw the right line of your rectangle
		for(uint8_t m = 1; m < y_length;m++){
			nokia_lcd_set_pixel(x-x_length+1,y-m,0);
		}
		//Fill the box
		for(uint8_t xyz = 0; xyz < x_length; xyz++){
			for(uint8_t fff = 0; fff < y_length-1;fff++){
				nokia_lcd_set_pixel(x-xyz,y-fff,0);
			}
		}
		
		//Draw the bottom line
		for(uint8_t ii = 0; ii < x_length ; ii++){
			nokia_lcd_set_pixel(x-ii,y-y_length+1,0);
		}
		
		
}
void nokia_lcd_draw_rectangle(uint8_t x , uint8_t y, uint8_t x_length, uint8_t y_length){
	

		//Draw top line 
	for(uint8_t i = 0; i < x_length ; i++){
		nokia_lcd_set_pixel(x-i,y,1);
	}
	
	//Draw the left line of the rectangle
	for(uint8_t j = 1 ;j < y_length ; j++){
		nokia_lcd_set_pixel(x,y-j,1);
	}
	
	//Draw the right line of your rectangle
	for(uint8_t m = 1; m < y_length;m++){
		nokia_lcd_set_pixel(x-x_length+1,y-m,1);
	}
		//Fill the box
	for(uint8_t xyz = 0; xyz < x_length; xyz++){
		for(uint8_t fff = 0; fff < y_length-1;fff++){
			nokia_lcd_set_pixel(x-xyz,y-fff,1);
		}
	}
	
	//Draw the bottom line
	for(uint8_t ii = 0; ii < x_length ; ii++){
		nokia_lcd_set_pixel(x-ii,y-y_length+1,1);
	}
	
	
	
	
}

//Returns false if the movement cannot be allowed since there is a pixel in that location area.
uint8_t nokia_lcd_check_hella_pixel(uint8_t x ,uint8_t y,uint8_t cur_type,uint8_t final_type){
	
	switch(cur_type){
		case 1:
			switch(final_type){
				case 3:
					for(uint8_t i = 0; i < 12;i++){
						for(uint8_t yy = 0; yy < 15;yy++){
							if(nokia_lcd_check_pixel(x+i-15,y-yy) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				case 2:
					for(uint8_t i = 0; i < 12;i++){
						for(uint8_t yy = 0; yy < 15;yy++){
							if(nokia_lcd_check_pixel(x-i+12,y-yy) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				
				default:
					break;
			}
		case 2:
			switch(final_type){
				case 1:
					for(uint8_t i = 0; i < 15 ;i++){
						for(uint8_t yy = 0; yy < 12;yy++){
							if(nokia_lcd_check_pixel(x-i,y-yy+12) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				default:
					break;
			}
			
		case 3:
			switch(final_type){
				case 1:
					for(uint8_t i = 0; i < 15;i++){
						for(uint8_t yy = 0; yy < 12;yy++){
							if(nokia_lcd_check_pixel(x-i,y-yy+12) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				default:
					break;
			}
		case 4:
			switch(final_type){
				case 5:
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-j-3) == 0){
								return 0;
							}
						}
					}
					
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x-i,y-6-j)){
								return 0;
							}
						}
					}
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-i-3,y-j) == 0){
								return 0;
							}
						}
					}
					return 1;
			}
		case 5:
			switch(final_type){
				case 4:
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-j) == 0){
								return 0;
							}
						}
					}
					
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-3-i,y-3-j) == 0){
								return 0;
							}
						}
					}
					
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-3-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					return 1;
				default:
					break;
			}
		case 6:
			switch(final_type){
				case 9:
				
					//The Top Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-j) == 0){
								return 0;
							}
						}
					}
					
					//The Top Right Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-3-i,y-j) == 0){
								return 0;
							}
						}
					}
					
					//The Bottom Right Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-3-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					
					//The Bottom Middle Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					return 1;
				case 8:
					
					//The Top Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-j) == 0){
								return 0;
							}
						}
					}
					
					
					
					//Bottom Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					//The Bottom Middle Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					//Bottom Right Corner 
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(nokia_lcd_check_pixel(x-i-3,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					
					//The Top Right Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-3-i,y-j) == 0){
								return 0;
							}
						}
					}
					return 1;
				case 7:
					//The Top Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-j) == 0){
								return 0;
							}
						}
					}
					
					
					
					//Bottom Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					//The Bottom Middle Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					//The Top Right Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-3-i,y-j) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				default:
					break;
			}
		case 7:
			switch(final_type){
				case 6:
					//The Top Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-j) == 0){
								return 0;
							}
						}
					}
					
					//Top right Corner
					for(uint8_t i = 0;i < 3; i++){
						for(uint8_t j = 0;j < 3 ; j++){
							if(nokia_lcd_check_pixel(x-3-i,y-j) == 0){
								return 0;
							}
						}
					}
					
					//Middle Right Corner
					for(uint8_t i = 0;i < 3 ;i++){
						for(uint8_t j = 0; j < 3;j++){
							if(nokia_lcd_check_pixel(x-i-3,y-3-j) == 0){
								return 0;
							}
						}
					}
					
					//Bottom Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				case 8:
				
					//The Top Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-j) == 0){
								return 0;
							}
						}
					}
					
					//Bottom Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					//Bottom Right Corner
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(nokia_lcd_check_pixel(x-i-3,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					//Middle Right Corner
					for(uint8_t i = 0;i < 3 ;i++){
						for(uint8_t j = 0; j < 3;j++){
							if(nokia_lcd_check_pixel(x-i-3,y-3-j) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				case 9:
					//The Top Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-j) == 0){
								return 0;
							}
						}
					}
					
					//Bottom Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x+3-i,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					//Bottom Right Corner
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(nokia_lcd_check_pixel(x-i-3,y-6-j) == 0){
								return 0;
							}
						}
					}
					
					//Middle Right Corner
					for(uint8_t i = 0;i < 3 ;i++){
						for(uint8_t j = 0; j < 3;j++){
							if(nokia_lcd_check_pixel(x-i-3,y-3-j) == 0){
								return 0;
							}
						}
					}
					
					//Top right Corner
					for(uint8_t i = 0;i < 3; i++){
						for(uint8_t j = 0;j < 3 ; j++){
							if(nokia_lcd_check_pixel(x-3-i,y-j) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				default:
					break;
			}
		case 8:
			switch(final_type){
				case 9:
					//Three way block
					//Bottom Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-i,y-3-j) == 0){
								return 0;
							}
						}
					}
					
					//Three way block
					//Upper Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0;j < 3;j++){
							if(nokia_lcd_check_pixel(x-i,y+3-j) == 0){
								return 0;
							}
						}
					}
					
					//Three way block
					//Upper Mid Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0;j < 3;j++){
							if(nokia_lcd_check_pixel(x-i-3,y+3-j) == 0){
								return 0;
							}
						}
					}
					
					//Three way block
					//Upper Right Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0;j < 3;j++){
							if(nokia_lcd_check_pixel(x-i-6,y+3-j) == 0){
								return 0;
							}
						}
					}
					
					//Three way block
					//Bottom Right Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0;j < 3;j++){
							if(nokia_lcd_check_pixel(x-i-6,y-3-j) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				case 7:
					
					//Triple block
					//Upper Left Corner
					for(uint8_t i = 0;i < 3 ;i++){
						for(uint8_t j = 0; j < 3;j++){
							if(nokia_lcd_check_pixel(x-i,y+3-j) == 0){
								return 0;
							}
						}
					}
					
					//Triple block
					//Upper Middle Block
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(nokia_lcd_check_pixel(x-i-3,y+3-j) == 0){
								return 0;
							}
						}
					}
					
					//Triple block
					//Upper Right Block
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(nokia_lcd_check_pixel(x-i-6,y+3-j) == 0){
								return 0;
							}
						}
					}
					
					//Three way block
					//Bottom Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-i,y-3-j) == 0){
								return 0;
							}
						}
					}
					
					//Three way block
					//Bottom Right Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-i-6,y-3-j) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				case 6:
					//Triple block
					//Upper Left Corner
					for(uint8_t i = 0;i < 3 ;i++){
						for(uint8_t j = 0; j < 3;j++){
							if(nokia_lcd_check_pixel(x-i,y+3-j) == 0){
								return 0;
							}
						}
					}
					
					//Triple block
					//Upper Middle Block
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(nokia_lcd_check_pixel(x-i-3,y+3-j) == 0){
								return 0;
							}
						}
					}
					
					//Triple block
					//Upper Right Block
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(nokia_lcd_check_pixel(x-i-6,y+3-j) == 0){
								return 0;
							}
						}
					}
					
					//Three way block
					//Bottom Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-i,y-3-j) == 0){
								return 0;
							}
						}
					}
					
					//Three way block
					//Bottom Right Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(nokia_lcd_check_pixel(x-i-6,y-3-j) == 0){
								return 0;
							}
						}
					}
					return 1;
					break;
				default:
					break;
			}
		case 9:
			switch(final_type){
				case 6:
					//The Middle Left
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x+3-i,y-3-j)){
								return 0;
							}
						}
					}
					
					//The Top Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x+3-i,y-j)){
								return 0;
							}
						}
					}
					
					//Top right Corner
					for(uint8_t i = 0;i < 3; i++){
						for(uint8_t j = 0;j < 3 ; j++){
							if(!nokia_lcd_check_pixel(x-3-i,y-j)){
								return 0;
							}
						}
					}
					
					//Bottom Right Corner
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(!nokia_lcd_check_pixel(x-i-3,y-6-j)){
								return 0;
							}
						}
					}
					return 1;
					break;
				case 7:
					//The Middle Left
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x+3-i,y-3-j)){
								return 0;
							}
						}
					}
					
					//The Top Left Corner
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x+3-i,y-j)){
								return 0;
							}
						}
					}
					
					//Top right Corner
					for(uint8_t i = 0;i < 3; i++){
						for(uint8_t j = 0;j < 3 ; j++){
							if(!nokia_lcd_check_pixel(x-3-i,y-j)){
								return 0;
							}
						}
					}
					
					//Bottom Right Corner
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(!nokia_lcd_check_pixel(x-i-3,y-6-j)){
								return 0;
							}
						}
					}
					
					//Bottom Left Corner
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x-i+3,y-6-j)){
								return 0;
							}
						}
					}
					return 1;
					break;
				case 8:
				
					//The Middle Left
					for(uint8_t i = 0;i < 3;i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x+3-i,y-3-j)){
								return 0;
							}
						}
					}
					
					//Bottom Left Corner
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x-i+3,y-6-j)){
								return 0;
							}
						}
					}
					
					
					
					//Bottom Right Corner
					for(uint8_t i = 0; i < 3 ; i++){
						for(uint8_t j = 0; j<3 ;j++){
							if(!nokia_lcd_check_pixel(x-i-3,y-6-j)){
								return 0;
							}
						}
					}
					
					//Top right Corner
					for(uint8_t i = 0;i < 3; i++){
						for(uint8_t j = 0;j < 3 ; j++){
							if(!nokia_lcd_check_pixel(x-3-i,y-j)){
								return 0;
							}
						}
					}
					return 1;
					break;
				default:
					break;
					
			}
		case 10:
			switch(final_type){
				default:
					return 0;
					break;
			}
		case 11:
			switch(final_type){
				case 12:
				
					//Top left Corner
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x+3-i,y-j)){
								return 0;
							}
						}
					}
					
					//Top Right Corner
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x-3-i,y-j)){
								return 0;
							}
						}
					}
					
					//Middle Right
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x-3-i,y-6-j)){
								return 0;
							}
						}
					}
					return 1;
					break;
				default:
					break;
					
			}
		case 12:
			switch(final_type){
				case 11:
				
					//Middle left
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x-i,y-3-j)){
								return 0;
							}
						}
					}
					
					//Top Right
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x-6-i,y-j)){
								return 0;
							}
						}
					}
					
					//Bottom Left
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x-i,y-6-j)){
								return 0;
							}
						}
					}
					
					//Bottom Middle
					for(uint8_t i = 0 ; i < 3;i++){
						for(uint8_t j = 0 ; j < 3 ;j++){
							if(!nokia_lcd_check_pixel(x-i-3,y-j-6)){
								return 0;
							}
						}
					}
					return 1;
					break;
				default:
					break;
			}
		default:
			break;
	}
	return 1;
}
void nokia_landed_lcd_render2(void){
	register unsigned i;
	/* Set column and row to 0 */
	write_cmd(0x80);
	write_cmd(0x40);

	/* Write screen to display */
	for (i = 0; i < 504; i++){
		nokia_lcd.screen[i] = nokia_lcd.landed[i];
	}
		//write_data(nokia_lcd.landed[i]);
}
void nokia_lcd_render(void)
{
	register unsigned i;
	/* Set column and row to 0 */
	write_cmd(0x80);
	write_cmd(0x40);

	/* Write screen to display */
	for (i = 0; i < 504; i++)
		write_data(nokia_lcd.screen[i]);
}
