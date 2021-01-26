#ifndef GAME_H
#define GAME_H
#include "common.h"

#define TICK_TIME_MS 800

int process_rotation(int x, int y, int n, int rotation);

int game(gameptr wgame);

#endif