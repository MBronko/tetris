#ifndef GAME_TOOLS_H
#define GAME_TOOLS_H
#include "common.h"

int get_rand_block_num();

block get_block(int n);

int process_rotation(int x, int y, int n, int rotation);

int is_legal_pos(gameptr game_data);

void shift_board(gameptr game_data, int line);

void get_new_block(gameptr game_data);

void remove_lines(gameptr game_data);

void place_block(gameptr game_data);

#endif
