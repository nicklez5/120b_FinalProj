#ifndef _BLOCK_LINE_H
#define _BLOCK_LINE_H
#include <avr/io.h>

//i would set the x and y with the cursor initially

struct Block_line{
	uint8_t coord_x;
	uint8_t coord_y;
	uint8_t object_type;
	uint8_t MAX_ROW;
	uint8_t MAX_COL;
	uint8_t finished_line; 
	int xyz[5][5];
	//uint8_t *ptr[MAX];
};

void choose_the_block(struct Block_line *tmp_blk);
//Rotate the block to the left
void rotate_lefts(struct Block_line *tmp_blk);

//Rotate the block to the right
void rotate_rights(struct Block_line *tmp_blk);

//Move the block to the left
void move_lefts(struct Block_line *tmp_blk);

//Move the block to the right
void move_rights(struct Block_line *tmp_blk);

void draw_me(struct Block_line *tmp_blk);
void keep_dropping_me(struct Block_line *tmp_blk);
uint8_t has_dropped(struct Block_line *tmp_blk);
uint8_t check_touch(struct Block_line *tmp_blk);
uint8_t check_back(uint8_t _front, uint8_t _back);
void drop_me(struct Block_line *tmp_blk);
void drop_down();
void destroy_line(struct Block_line *holy_field);
uint8_t all_fail();
uint8_t random_number();
void refresh_boiz();
#endif