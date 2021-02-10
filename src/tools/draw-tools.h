#ifndef DRAW_TOOLS_H
#define DRAW_TOOLS_H
int center_y(int box_y);

int center_x(int box_x);

int center_text(WINDOW *win, char* string);

int check_terminal_size();

void del_game_win(gameptr game_data);

void draw_legend(gameptr game_data);

void draw_colored(WINDOW *win, int y, int x, int color, char str[]);

#endif
