#ifndef DRAW_H
#define DRAW_H
#include <curses.h>
#include "common.h"

void draw_menu(WINDOW *menu, int highlight, int game_active, char *choices[], int n_choices);

void draw_board(WINDOW *board, game_data data);

void draw_scoreboard(WINDOW *scoreboard, int score);

void draw_legend(WINDOW *legend);

void draw_next_block(WINDOW *block_window, block next_block);

void draw_game(WINDOW *game, game_data data);

void window_resize(WINDOW *window);

#endif