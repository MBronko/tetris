#include <ncurses.h>
#include <string.h>
#include "common.h"
#include "draw-tools.h"
#include "../components/game.h"

int center_y(int box_y){
    int y = getmaxy(stdscr);
    int res = (y-(box_y))/2;
    return max(res, 0);
}

int center_x(int box_x){
    int x = getmaxx(stdscr);
    int res = (x-(box_x))/2;
    return max(res, 0);
}

int center_text(WINDOW *win, char* string){
    int x = getmaxx(win);
    int res = (x-strlen(string))/2;
    return max(res, 0);
}

int check_terminal_size(){
    int x, y;
    char msg[] = "Window is too small";
    getmaxyx(stdscr, y, x);
    if(x>=MIN_WINDOW_WIDTH && y>=MIN_WINDOW_HEIGHT){
        return 1;
    }
    clear();
    mvprintw(center_y(0), center_text(stdscr, msg), msg);
    return 0;
}

void draw_colored(WINDOW *win, int y, int x, int color, char str[]){
    wattron(win, COLOR_PAIR(global_colors[color]));
    mvwprintw(win, y, x, str);
    wattroff(win, COLOR_PAIR(global_colors[color]));
}

void del_game_win(gameptr game_data){
    delwin(game_data->win_board);
    game_data->win_board = NULL;
    delwin(game_data->win_next);
    game_data->win_next = NULL;
    delwin(game_data->win_legend);
    game_data->win_legend = NULL;
    delwin(game_data->win_score);
    game_data->win_score = NULL;
}