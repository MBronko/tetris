#include <ncurses.h>
#include <string.h>
#include "common.h"
#include "draw-tools.h"

int center_y(int box_y){
    int y = getmaxy(stdscr);
    int res = (y-(box_y))/2;
    return res >= 0 ? res : 0;
}

int center_x(int box_x){
    int x = getmaxx(stdscr);
    int res = (x-(box_x))/2;
    return res >= 0 ? res : 0;
}

int center_text(WINDOW *win, char* string){
    int x = getmaxx(win);
    int res = (x-strlen(string))/2;
    return res >= 0 ? res : 0;
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

void draw_legend(gameptr game_data){
    int y=1;
    char title[] = "Legend";
    char *content[] = {
            "LEFT      \xe2\x86\x90 A",
            "RIGHT     \xe2\x86\x92 D",
            "ROTATE\xE2\x86\xBB   \xe2\x86\x91 X",
            "ROTATE\xE2\x86\xBA     z",
            "DOWN      \xe2\x86\x93 S",
            "PAUSE       P",
            "QUIT        Q",
    };
    int n_content = sizeof(content)/sizeof(content[0]);
    box(game_data->win_legend, 0, 0);
    mvwprintw(game_data->win_legend, 0, center_text(game_data->win_legend, title), title);
    for(int i=0; i<n_content; i++){
        if(i == 5) y+=2;
        mvwprintw(game_data->win_legend, y, 1, content[i]);
        y++;
    }
    wrefresh(game_data->win_legend);
}