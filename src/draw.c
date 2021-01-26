#include <curses.h>
#include <string.h>

#include "common.h"
#include "menu.h"
#include "draw.h"

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

void draw_menu(menuptr wmenu){
    int y = 2;
    box(wmenu->win, 0, 0);

    wattron(wmenu->win, A_BOLD);
    mvwprintw(wmenu->win, y, center_text(wmenu->win, wmenu->rand_header), "%s", wmenu->rand_header);
    wattroff(wmenu->win, A_BOLD);
    y+=2;

    for (int i = 0; i < n_options; ++i) {
        int x = center_text(wmenu->win, options[i]);
        if (!wmenu->game_active && i == 0) {
            wattron(wmenu->win, A_DIM);
            mvwprintw(wmenu->win, y, x, "%s", options[i]);
            wattroff(wmenu->win, A_DIM);

        } else if (wmenu->highlight == i + 1) {
            wattron(wmenu->win, A_REVERSE);
            mvwprintw(wmenu->win, y, x, "%s", options[i]);
            wattroff(wmenu->win, A_REVERSE);
        } else
            mvwprintw(wmenu->win, y, x, "%s", options[i]);
        y++;
    }
    clear();
    refresh();
    wrefresh(wmenu->win);
}

void draw_board(gameptr wgame){
    char title[] = "Board";
    box(wgame->win_board, 0, 0);
    mvwprintw(wgame->win_board, 0, center_text(wgame->win_board, title), title);
    wrefresh(wgame->win_board);
}

void draw_next_block(gameptr wgame){
    char title[] = "Next";
    box(wgame->win_next, 0, 0);
    mvwprintw(wgame->win_next, 0, center_text(wgame->win_next, title), title);
    wrefresh(wgame->win_next);
}

void draw_legend(gameptr wgame){
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
    box(wgame->win_legend, 0, 0);
    mvwprintw(wgame->win_legend, 0, center_text(wgame->win_legend, title), title);
    for(int i=0; i<n_content; i++){
        if(i == 5) y+=2;
        mvwprintw(wgame->win_legend, y, 1, content[i]);
        y++;
    }
    wrefresh(wgame->win_legend);
}

void draw_scoreboard(gameptr wgame){
    char title[] = "Score";
    int score = 9999;
    char score_str[7];
    box(wgame->win_score, 0, 0);
    sprintf(score_str, "%d", score);
    mvwprintw(wgame->win_score, 0, center_text(wgame->win_score, title), title);
    mvwprintw(wgame->win_score, 1, getmaxx(wgame->win_score)-strlen(score_str)-1, score_str);

    wrefresh(wgame->win_score);
}


void draw_game(gameptr wgame){
    clear();
    refresh();
    draw_board(wgame);
    draw_next_block(wgame);
    draw_legend(wgame);
    draw_scoreboard(wgame);
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

void del_game_win(gameptr wgame){
    delwin(wgame->win_board);
    wgame->win_board = NULL;
    delwin(wgame->win_next);
    wgame->win_next = NULL;
    delwin(wgame->win_legend);
    wgame->win_legend = NULL;
    delwin(wgame->win_score);
    wgame->win_score = NULL;
}

void game_resize(gameptr wgame){
    if(check_terminal_size()){
        int new_y = center_y(MIN_WINDOW_HEIGHT);
        int new_x = center_x(MIN_WINDOW_WIDTH);

        int panel_x = new_x + BOARD_WIDTH + SPACE_BETWEEN_WINDOWS;
        int legend_y = new_y + BLOCK_WINDOW_HEIGHT + SPACE_BETWEEN_WINDOWS;
        int scoreboard_y = legend_y + SPACE_BETWEEN_WINDOWS + LEGEND_WINDOW_HEIGHT;

//        if(wgame->win_board != NULL && getmaxy(stdscr)>MIN_WINDOW_HEIGHT+1){
//            if(wgame->win_x != new_x || wgame->win_y != new_y){
//                mvwin(wgame->win_board, new_y, new_x);
//                mvwin(wgame->win_next, new_y, panel_x);
//                mvwin(wgame->win_legend, legend_y, panel_x);
//                mvwin(wgame->win_score, scoreboard_y, panel_x);
//            }
//        }
//        else{
            wgame->win_board = newwin(BOARD_HEIGHT_VISIBLE+BOX_BORDER, BOARD_WIDTH, new_y, new_x);
            wgame->win_next = newwin(BLOCK_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, new_y, panel_x);
            wgame->win_legend = newwin(LEGEND_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, legend_y, panel_x);
            wgame->win_score= newwin(SCORE_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, scoreboard_y, panel_x);
//        }
        wgame->win_x = new_x;
        wgame->win_y = new_y;
        draw_game(wgame);
    }
    else if(wgame->win_board != NULL){
        del_game_win(wgame);
    }
}

void menu_resize(menuptr wmenu){
    if(check_terminal_size()){
        int new_y = center_y(MENU_HEIGHT);
        int new_x = center_x(MENU_WIDTH);
        if(wmenu->win != NULL){
            if(wmenu->win_x != new_x || wmenu->win_y != new_y) {
                mvwin(wmenu->win, new_y, new_x);
            }
        }
        else{
            wmenu->win = newwin(MENU_HEIGHT,MENU_WIDTH,new_y,new_x);
        }
        wmenu->win_x = new_x;
        wmenu->win_y = new_y;
        draw_menu(wmenu);
    }
    else if(wmenu->win != NULL){
        delwin(wmenu->win);
        wmenu->win = NULL;
    }
}