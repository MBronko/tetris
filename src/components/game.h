#ifndef GAME_H
#define GAME_H
#include "../tools/common.h"

#define TICK_TIME_MS 700
#define MINI_TICK_TIME_MS 100

extern const short global_colors[];
extern const int global_block_count;
extern block global_blocks[];
extern bool global_jumped;
extern int global_game_state;

int process_rotation(int x, int y, int n, int rotation);

int game(gameptr wgame);

#endif