#ifndef DRAW_GAME_H
#define DRAW_GAME_H


void draw_board(gameptr game_data);

void draw_next_block(gameptr game_data);

void draw_scoreboard(gameptr game_data);

void draw_game(gameptr game_data);

void game_resize(gameptr game_data);

void gameover_view(gameptr game_data);

#endif