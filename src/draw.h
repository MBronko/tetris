#ifndef DRAW_H
#define DRAW_H
#include <curses.h>
#include "common.h"

extern char *options[];
extern char *rand_header;
extern int n_options;
extern int highlight;
extern int game_active;
extern WINDOW* wmenu;

int center_y(int box_y);

int center_x(int box_x);

void draw_menu();

void draw_board(WINDOW *board, game_data data);

void draw_scoreboard(WINDOW *scoreboard, int score);

void draw_legend(WINDOW *legend);

void draw_next_block(WINDOW *block_window, block next_block);

void draw_game(viewptr wgame, dataptr data);

void game_resize(viewptr view, dataptr data);

int menu_resize();

#endif