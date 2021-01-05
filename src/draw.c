#include "draw.h"
#include <curses.h>
#include "common.h"
#include <stdlib.h>
//#include <stdio.h>
//#include <time.h>

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

void draw_menu(){
    wclear(wmenu);

    int x = 2, y = 2;
    box(wmenu, 0, 0);

    wattron(wmenu, A_BOLD);
    mvwprintw(wmenu, y, x, "%s", rand_header);
    wattroff(wmenu, A_BOLD);
    y+=2;

    for (int i = 0; i < n_options; ++i) {
        if (!game_active && i == 0) {
            wattron(wmenu, A_DIM);
            mvwprintw(wmenu, y, x, "%s", options[i]);
            wattroff(wmenu, A_DIM);

        } else if (highlight == i + 1) {
            wattron(wmenu, A_REVERSE);
            mvwprintw(wmenu, y, x, "%s", options[i]);
            wattroff(wmenu, A_REVERSE);
        } else
            mvwprintw(wmenu, y, x, "%s", options[i]);
        y++;
    }
    clear();
    refresh();
    wrefresh(wmenu);
}

void draw_board(WINDOW *board, game_data data){

}

void draw_scoreboard(WINDOW *scoreboard, int score){

}

void draw_legend(WINDOW *legend){

}

void draw_next_block(WINDOW *block_window, block next_block){

}

void draw_game(viewptr wgame, dataptr data){
    int x, y;
    getmaxyx(stdscr, y, x);
    int left_x = (x - MIN_WINDOW_WIDTH)/2;
    int top_y = (y - BOARD_HEIGHT_VISIBLE) / 2;
    int panel_x = left_x + BOARD_WIDTH + SPACE_BETWEEN_WINDOWS;
//    WINDOW * test = newwin(20, 20, 1, 1);
//    refresh();
//    mvwin(test, 3, 10);
//    box(test, 0, 0);
//    wrefresh(test);

//    printw("%d %d %d %d %d", x, MIN_WINDOW_WIDTH, (x-MIN_WINDOW_WIDTH), (x-MIN_WINDOW_WIDTH)/2, left_x);

    mvwin(wgame->board, top_y, left_x);
    box(wgame->board, 0, 0);

    mvwin(wgame->next_block, top_y, panel_x);
    box(wgame->next_block, 0, 0);

    int legend_y = top_y+SPACE_BETWEEN_WINDOWS+BLOCK_WINDOW_HEIGHT;
    mvwin(wgame->legend, legend_y, panel_x);
    box(wgame->legend, 0, 0);

    int scoreboard_y = legend_y + SPACE_BETWEEN_WINDOWS + LEGEND_WINDOW_HEIGHT;
    mvwin(wgame->scoreboard, scoreboard_y, panel_x);
    box(wgame->scoreboard, 0, 0);

    wrefresh(wgame->board);
    wrefresh(wgame->next_block);
    wrefresh(wgame->legend);
    wrefresh(wgame->scoreboard);
}

void game_resize(viewptr view, dataptr data){

}

int menu_resize(){
    int x, y;
    getmaxyx(stdscr, y, x);
    char msg[] = "Window is too small";
    int msg_len = sizeof(msg)/sizeof(msg[0]);

    if(x>=MIN_WINDOW_WIDTH && y>=BOARD_HEIGHT_VISIBLE){
        int new_y = center_y(MENU_HEIGHT);
        int new_x = center_x(MENU_WIDTH);
        if(wmenu != NULL){
            mvwin(wmenu, new_y, new_x);
        }
        else{
            wmenu = newwin(MENU_HEIGHT,MENU_WIDTH,new_y,new_x);
        }
        draw_menu();
        return 1;
    }
    clear();
    mvprintw(center_y(0), center_x(msg_len), msg);
    delwin(wmenu);
    wmenu = NULL;
    return 0;
}