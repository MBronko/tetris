#ifndef DRAW_H
#define DRAW_H
#include <curses.h>
#include "common.h"

void draw_menu(WINDOW *wmenu, menuptr menu_data);

void draw_board(WINDOW *board, game_data data);

void draw_scoreboard(WINDOW *scoreboard, int score);

void draw_legend(WINDOW *legend);

void draw_next_block(WINDOW *block_window, block next_block);

void draw_game(viewptr wgame, dataptr data);

void game_resize(viewptr view, dataptr data);

WINDOW * menu_resize(WINDOW * reset, menuptr menu_data);

#endif