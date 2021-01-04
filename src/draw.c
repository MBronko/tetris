#include "draw.h"
#include <curses.h>
#include "common.h"

void draw_menu(WINDOW *menu, int highlight, int game_active, char *choices[], int n_choices){
    int x, y;
    x = 2;
    y = 2;
    box(stdscr, 0, 0);
    for (int i = 0; i < n_choices; ++i) {
        if (!game_active && i == 0) {
            attron(A_DIM);
            mvwprintw(stdscr, y, x, "%s", choices[i]);
            attroff(A_DIM);

        } else if (highlight == i + 1) {
            attron(A_REVERSE);
            mvwprintw(stdscr, y, x, "%s", choices[i]);
            attroff(A_REVERSE);
        } else
            mvwprintw(stdscr, y, x, "%s", choices[i]);
        ++y;
    }
    refresh();
}

void draw_board(WINDOW *board, game_data data){

}

void draw_scoreboard(WINDOW *scoreboard, int score){

}

void draw_legend(WINDOW *legend){

}

void draw_next_block(WINDOW *block_window, block next_block){

}

void draw_game(WINDOW *game, game_data data){

}

void window_resize(WINDOW *window){

}