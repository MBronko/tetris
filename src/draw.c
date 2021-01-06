#include "draw.h"
#include <curses.h>
#include "common.h"
#include <stdlib.h>
#include <string.h>
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

int center_text(WINDOW *win, char* string){
    int x = getmaxx(win);
    int res = (x-strlen(string))/2;
    return res >= 0 ? res : 0;
}

int get_int_len(int x){
    if(x<1) return 1;
    int res = 0;
    while(x > 0){
        res++;
        x /= 10;
    }
    return res;
}

void draw_menu(){
    int y = 2;
    clear();
    box(wmenu, 0, 0);

    wattron(wmenu, A_BOLD);
    mvwprintw(wmenu, y, center_text(wmenu, rand_header), "%s", rand_header);
    wattroff(wmenu, A_BOLD);
    y+=2;

    for (int i = 0; i < n_options; ++i) {
        int x = center_text(wmenu, options[i]);
//        printw("%d\n", x);
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
    refresh();
    wrefresh(wmenu);
}

void draw_board(){
    char title[] = "Board";
    box(board, 0, 0);
    mvwprintw(board, 0, center_text(board, title), title);
    wrefresh(board);
}

void draw_next_block(){
    char title[] = "Next";
    box(next_block, 0, 0);
    mvwprintw(next_block, 0, center_text(next_block, title), title);
    wrefresh(next_block);
}

void draw_legend(){
    int y=1;
    char title[] = "Legend";
    char *content[] = {
            "LEFT      \xe2\x86\x90 A",
            "RIGHT     \xe2\x86\x92 D",
            "ROTATE\xE2\x86\xBB   \xe2\x86\x91 W",
            "ROTATE\xE2\x86\xBA   \xe2\x86\x93 S",
            "DOWN    SPACE",
            "PAUSE       P",
            "QUIT        Q",
    };
    int n_content = sizeof(content)/sizeof(content[0]);
    box(legend, 0, 0);
    mvwprintw(legend, 0, center_text(legend, title), title);
    for(int i=0; i<n_content; i++){
        if(i == 5) y+=2;
        mvwprintw(legend, y, 1, content[i]);
        y++;
    }
    wrefresh(legend);
}

void draw_scoreboard(){
    char title[] = "Score";
    int score = 9999;
    char score_str[7];
    box(scoreboard, 0, 0);
    sprintf(score_str, "%d", score);
    mvwprintw(scoreboard, 0, center_text(scoreboard, title), title);
    mvwprintw(scoreboard, 1, getmaxx(scoreboard)-strlen(score_str)-1, score_str);
    wrefresh(scoreboard);
}


void draw_game(){
    clear();
    refresh();
    draw_board();
    draw_next_block();
    draw_legend();
    draw_scoreboard();
}

int check_terminal_size(){
    int x, y;
    char msg[] = "Window is too small";
    int msg_len = strlen(msg);
    getmaxyx(stdscr, y, x);
    if(x>=MIN_WINDOW_WIDTH && y>=BOARD_HEIGHT_VISIBLE){
        return 1;
    }
    clear();
    mvprintw(center_y(0), center_x(msg_len), msg);
    return 0;
}

void del_game_win(){
    delwin(board);
    board = NULL;
    delwin(next_block);
    next_block = NULL;
    delwin(legend);
    legend = NULL;
    delwin(scoreboard);
    scoreboard = NULL;

//    delwin(wgame);
//    wgame = NULL;
}

void game_resize(){
    if(check_terminal_size()){
        int new_y = center_y(BOARD_HEIGHT_VISIBLE);
        int new_x = center_x(MIN_WINDOW_WIDTH);
        printw("xd %d %d\n", new_y, new_x);
        int panel_x = new_x + BOARD_WIDTH + SPACE_BETWEEN_WINDOWS;
        int legend_y = new_y + BLOCK_WINDOW_HEIGHT + SPACE_BETWEEN_WINDOWS;
        int scoreboard_y = legend_y + SPACE_BETWEEN_WINDOWS + LEGEND_WINDOW_HEIGHT;

        if(wgame != NULL){
//            mvwin(wgame, new_y, new_x);

            mvwin(board, new_y, new_x);
            mvwin(next_block, new_y, panel_x);
            mvwin(legend, legend_y, panel_x);
            mvwin(scoreboard, scoreboard_y, panel_x);
        }
        else{

            board = newwin(BOARD_HEIGHT_VISIBLE, BOARD_WIDTH, new_y, new_x);
            next_block = newwin(BLOCK_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, new_y, panel_x);
            legend = newwin(LEGEND_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, legend_y, panel_x);
            scoreboard = newwin(SCORE_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, scoreboard_y, panel_x);

//            wgame = newwin(BOARD_HEIGHT_VISIBLE, MIN_WINDOW_WIDTH, new_y, new_x);
//
//            board = subwin(wgame, BOARD_HEIGHT_VISIBLE, BOARD_WIDTH, 0, 0);
//            next_block = subwin(wgame, BLOCK_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, panel_x);
//            legend = subwin(wgame, LEGEND_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, legend_y, panel_x);
//            scoreboard = subwin(wgame, SCORE_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, scoreboard_y, panel_x);
        }
        draw_game();
    }
    else{
        del_game_win();
    }
}

void menu_resize(){
    if(check_terminal_size(wmenu)){
        int new_y = center_y(MENU_HEIGHT);
        int new_x = center_x(MENU_WIDTH);
        if(wmenu != NULL){
            mvwin(wmenu, new_y, new_x);
        }
        else{
            wmenu = newwin(MENU_HEIGHT,MENU_WIDTH,new_y,new_x);
        }
        draw_menu();
    }
    else{
        delwin(wmenu);
        wmenu = NULL;
    }
}