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

void draw_board(gameptr game_data){
    char title[] = "Board";
    box(game_data->win_board, 0, 0);
    mvwprintw(game_data->win_board, 0, center_text(game_data->win_board, title), title);
    for (int y = 0; y < BOARD_GAME_HEIGHT; y++) {
        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
            if(game_data->board[y][x] != -1){
                wattron(game_data->win_board, COLOR_PAIR(colors[game_data->board[y][x]]));
                mvwprintw(game_data->win_board, BOARD_GAME_HEIGHT-y, 2*x+1, "  ");
                wattroff(game_data->win_board, COLOR_PAIR(colors[game_data->board[y][x]]));
            }
            else{
                mvwprintw(game_data->win_board, BOARD_GAME_HEIGHT-y, 2*x+1, " .");
            }
        }
    }

//    draw actual block
//    int x = game_data->act_block.x;
//    int y = game_data->act_block.y;
    int n = game_data->act_block.n;
    int rotation = game_data->act_block.rotation;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if(y+game_data->act_block.y < BOARD_GAME_HEIGHT &&
                game_data->act_block.board[process_rotation(x, y, n, rotation)]){
                wattron(game_data->win_board, COLOR_PAIR(colors[game_data->act_block.color]));
                mvwprintw(game_data->win_board, BOARD_GAME_HEIGHT-(y+game_data->act_block.y), 2*(x+game_data->act_block.x)+1, "  ");
                wattroff(game_data->win_board, COLOR_PAIR(colors[game_data->act_block.color]));
            }
        }
    }

    wrefresh(game_data->win_board);
}

void draw_next_block(gameptr game_data){
    wclear(game_data->win_next);
    char title[] = "Next";
    box(game_data->win_next, 0, 0);
    mvwprintw(game_data->win_next, 0, center_text(game_data->win_next, title), title);

    block next_block = blocks[game_data->next_block];
    int n = next_block.n;

//    offset for cosmetic purpose
    int y_off = n == 4 ? 1 : 0;

    if(n==3){
        int check = 1;
        for (int x = 0; x < n; x++) {
            if(next_block.board[x]) check = 0;
        }
        if(check) y_off++;
    }

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if(next_block.board[n*y+x]){
                wattron(game_data->win_next, COLOR_PAIR(colors[game_data->next_block]));
                mvwprintw(game_data->win_next, (BLOCK_WINDOW_HEIGHT)/2-y+y_off, 2*x+7-n, "  ");
                wattroff(game_data->win_next, COLOR_PAIR(colors[game_data->next_block]));
            }
        }
    }

    wrefresh(game_data->win_next);
}

void draw_legend(gameptr game_data){
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
    box(game_data->win_legend, 0, 0);
    mvwprintw(game_data->win_legend, 0, center_text(game_data->win_legend, title), title);
    for(int i=0; i<n_content; i++){
        if(i == 5) y+=2;
        mvwprintw(game_data->win_legend, y, 1, content[i]);
        y++;
    }
    wrefresh(game_data->win_legend);
}

void draw_scoreboard(gameptr game_data){
    char title[] = "Score";
    char score_str[7];
    box(game_data->win_score, 0, 0);
    sprintf(score_str, "%d", game_data->score);
    mvwprintw(game_data->win_score, 0, center_text(game_data->win_score, title), title);
    mvwprintw(game_data->win_score, 1, getmaxx(game_data->win_score)-strlen(score_str)-1, score_str);

    wrefresh(game_data->win_score);
}


void draw_game(gameptr game_data){
    clear();
    refresh();
    draw_board(game_data);
    draw_next_block(game_data);
    draw_legend(game_data);
    draw_scoreboard(game_data);
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

void game_resize(gameptr game_data){
    if(check_terminal_size()){
        int new_y = center_y(MIN_WINDOW_HEIGHT);
        int new_x = center_x(MIN_WINDOW_WIDTH);

        int panel_x = new_x + BOARD_WIN_WIDTH + SPACE_BETWEEN_WINDOWS;
        int legend_y = new_y + BLOCK_WINDOW_HEIGHT + SPACE_BETWEEN_WINDOWS;
        int scoreboard_y = legend_y + SPACE_BETWEEN_WINDOWS + LEGEND_WINDOW_HEIGHT;

        if(game_data->win_board != NULL && getmaxy(stdscr)>MIN_WINDOW_HEIGHT+1){
            if(game_data->win_x != new_x || game_data->win_y != new_y){
                mvwin(game_data->win_board, new_y, new_x);
                mvwin(game_data->win_next, new_y, panel_x);
                mvwin(game_data->win_legend, legend_y, panel_x);
                mvwin(game_data->win_score, scoreboard_y, panel_x);
            }
        }
        else{
            game_data->win_board = newwin(MIN_WINDOW_HEIGHT, BOARD_WIN_WIDTH, new_y, new_x);
            game_data->win_next = newwin(BLOCK_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, new_y, panel_x);
            game_data->win_legend = newwin(LEGEND_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, legend_y, panel_x);
            game_data->win_score= newwin(SCORE_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, scoreboard_y, panel_x);
        }
        game_data->win_x = new_x;
        game_data->win_y = new_y;
        draw_game(game_data);
    }
    else if(game_data->win_board != NULL){
        del_game_win(game_data);
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

void game_over_view(){
//    jakiś basic widok z napisem game over
}