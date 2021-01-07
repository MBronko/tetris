#ifndef DRAW_H
#define DRAW_H
#include <ncurses.h>
#include "common.h"

void draw_menu(menuptr wmenu);

void draw_game(gameptr wgame);

void del_game_win();

void game_resize(gameptr wgame);

void menu_resize(menuptr wmenu);

#endif