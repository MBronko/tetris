#ifndef GAME_H
#define GAME_H
#include "../tools/common.h"

#define TICK_TIME_MS 700
#define MINI_TICK_TIME_MS 100

extern short colors[];
extern int block_count;
extern block blocks[];
extern int quit;
extern int jumped;

int process_rotation(int x, int y, int n, int rotation);

int game(gameptr wgame);

#endif