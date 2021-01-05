#include "draw.h"
#include <curses.h>
#include "common.h"
#include <stdlib.h>
//#include <stdio.h>
//#include <time.h>

void draw_menu(WINDOW *wmenu, menuptr menu_data){
    wclear(wmenu);
//    char *choices[] = {
//            "Resume",
//            "New Game",
//            "Quit",
//    };
//    int n_choices = sizeof(choices) / sizeof(choices[0]);


    int x = 2, y = 2;
    box(wmenu, 0, 0);

    wattron(wmenu, A_BOLD);
    mvwprintw(wmenu, y, x, "%s", menu_data->rand_header);
    wattroff(wmenu, A_BOLD);
    y+=2;

    for (int i = 0; i < menu_data->n_options; ++i) {
        if (!menu_data->game_active && i == 0) {
            wattron(wmenu, A_DIM);
            mvwprintw(wmenu, y, x, "%s", menu_data->options[i]);
            wattroff(wmenu, A_DIM);

        } else if (menu_data->highlight == i + 1) {
            wattron(wmenu, A_REVERSE);
            mvwprintw(wmenu, y, x, "%s", menu_data->options[i]);
            wattroff(wmenu, A_REVERSE);
        } else
            mvwprintw(wmenu, y, x, "%s", menu_data->options[i]);
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

WINDOW * menu_resize(WINDOW * reset, menuptr menu_data){
    int x, y, new_x, new_y;
    char msg[] = "Window is too small";

    getmaxyx(stdscr, y, x);
    if(x>=MIN_WINDOW_WIDTH && y>=BOARD_HEIGHT_VISIBLE){
        new_y = (y-MENU_HEIGHT)/2;
        new_x = (x-MENU_WIDTH)/2;
        if(reset != NULL){
            mvwin(reset, new_y, new_x);
        }
        else{
            reset = newwin(MENU_HEIGHT,MENU_WIDTH,new_y,new_x);
        }
        draw_menu(reset, menu_data);
        return reset;
    };
    int msg_len = sizeof(msg)/sizeof(msg[0]);
    new_x = x-msg_len >= 0 ? (x-msg_len)/2 : 0;
    new_y = y >= 0 ? y/2 : 0;
    clear();
    mvprintw(new_y, new_x, msg);
    delwin(reset);
    return NULL;
}