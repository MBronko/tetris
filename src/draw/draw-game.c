#include "ncurses.h"
#include <string.h>

#include "../tools/common.h"
#include "../tools/draw-tools.h"
#include "draw-game.h"
#include "../components/game.h"

void draw_board(gameptr game_data, bool draw_whole_board){
    char title[] = "Board";
    int min_y, max_y, min_x, max_x;

//    if its not necessary draw only around actual block
    if(draw_whole_board){
        box(game_data->win_board, 0, 0);
        mvwprintw(game_data->win_board, 0, center_text(game_data->win_board, title), title);
        min_y = 0;
        max_y = BOARD_GAME_HEIGHT;
        min_x = 0;
        max_x = BOARD_GAME_WIDTH;
    }
    else{
        min_y = max(game_data->act_block.y - 1, 0);
        max_y = min(game_data->act_block.y + game_data->act_block.n + 1, BOARD_GAME_HEIGHT);
        min_x = max(game_data->act_block.x - 1, 0);
        max_x = min(game_data->act_block.x + game_data->act_block.n + 1, BOARD_GAME_WIDTH);
    }

//    draw dotted grid
    for (int y = min_y; y < max_y; y++) {
        for (int x = min_x; x < max_x; x++) {
            mvwprintw(game_data->win_board, BOARD_GAME_HEIGHT - y, 2*x+1, " .");
        }
    }

//    draw board blocks
    for (int y = min_y; y < max_y; y++) {
        for (int x = min_x; x < max_x; x++) {
            if(game_data->board[y][x] != -1){
                draw_colored(game_data->win_board, BOARD_GAME_HEIGHT-y, 2*x+1, game_data->board[y][x],"  ");
            }
        }
    }

//    draw actual block
    int n = game_data->act_block.n;
    int rotation = game_data->act_block.rotation;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if(y+game_data->act_block.y < BOARD_GAME_HEIGHT && game_data->act_block.board[process_rotation(x, y, n, rotation)]){
                draw_colored(game_data->win_board, BOARD_GAME_HEIGHT-(y+game_data->act_block.y), 2*(x+game_data->act_block.x)+1, game_data->act_block.color, "  ");
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

    block next_block = global_blocks[game_data->next_block];
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
//                                           magic numbers to make blocks in the middle of box
                draw_colored(game_data->win_next, (BLOCK_WINDOW_HEIGHT)/2-y+y_off, 2*x+7-n, game_data->next_block, "  ");
            }
        }
    }

    wrefresh(game_data->win_next);
}

void draw_legend(gameptr game_data){
    int y=1;

//    make additional 2 lines empty space before item with this index
    int space_index = 5;
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
        if(i == space_index) y+=2;
        mvwprintw(game_data->win_legend, y, 1, content[i]);
        y++;
    }
    wrefresh(game_data->win_legend);
}

void draw_scoreboard(gameptr game_data){
    wclear(game_data->win_score);

    char title[] = "Score";
    char score_str[20];
    box(game_data->win_score, 0, 0);
    sprintf(score_str, "%d", game_data->score);
    mvwprintw(game_data->win_score, 0, center_text(game_data->win_score, title), title);
    mvwprintw(game_data->win_score, 1, getmaxx(game_data->win_score)-strlen(score_str)-1, score_str);

    wrefresh(game_data->win_score);
}

void draw_game(gameptr game_data){
    clear();
    refresh();
    draw_board(game_data, true);
    draw_next_block(game_data);
    draw_legend(game_data);
    draw_scoreboard(game_data);
}

void game_resize(gameptr game_data){
    if(check_terminal_size()){
        int new_y = center_y(MIN_WINDOW_HEIGHT);
        int new_x = center_x(MIN_WINDOW_WIDTH);

        int panel_x = new_x + BOARD_WIN_WIDTH + SPACE_BETWEEN_WINDOWS;
        int legend_y = new_y + BLOCK_WINDOW_HEIGHT + SPACE_BETWEEN_WINDOWS;
        int scoreboard_y = legend_y + SPACE_BETWEEN_WINDOWS + LEGEND_WINDOW_HEIGHT;

        if(game_data->win_board != NULL && getmaxy(stdscr)>MIN_WINDOW_HEIGHT+1){
//            if window is resized but game drawing shouldn't be changed then ignore (happens because of dividing int by 2)
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

void gameover_view(gameptr game_data){
    clear();

    char msg[20] = "GAME OVER";
    char score_msg[20] = "Score: ";
    char submsg[20] = "Press ENTER to exit";
    int y = center_y(0);

    char score_str[20];
    sprintf(score_str, "%d", game_data->score);
    strcat(score_msg, score_str);

    mvprintw(y-2, center_text(stdscr, msg), msg);
    mvprintw(y, center_text(stdscr, score_msg), score_msg);
    mvprintw(y+2, center_text(stdscr, submsg), submsg);
    refresh();
}