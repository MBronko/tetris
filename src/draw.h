#ifndef DRAW_H
#define DRAW_H
#include <ncurses.h>
#include "common.h"


extern char *options[];
extern char *rand_header;
extern int n_options;
extern int highlight;
extern int game_active;
extern WINDOW* wmenu;
extern WINDOW *wgame, *board, *next_block, *legend, *scoreboard;

int center_y(int box_y);

int center_x(int box_x);

int center_text(WINDOW *win, char* string);

int get_int_len(int x);

void draw_menu();

void draw_board();

void draw_scoreboard();

void draw_legend();

void draw_next_block();

void draw_game();

int check_terminal_size();

void del_game_win();

void game_resize();

void menu_resize();

#endif