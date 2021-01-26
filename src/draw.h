#ifndef DRAW_H
#define DRAW_H
#include <ncurses.h>
#include "common.h"
#include "game.h"

extern short colors[];
extern int block_count;
extern block blocks[];

void draw_menu(menuptr wmenu);

void draw_board(gameptr game_data);

void draw_next_block(gameptr game_data);

void draw_scoreboard(gameptr game_data);

void draw_game(gameptr wgame);

void del_game_win();

void game_resize(gameptr wgame);

void menu_resize(menuptr wmenu);

#endif