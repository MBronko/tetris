#include <stdlib.h>
#include "ncurses.h"

#include "common.h"
#include "../components/menu.h"
#include "menu-tools.h"
#include "draw-tools.h"
#include "../components/game.h"

void reset_game_data(gameptr data) {
    data->next_block = -1;
    data->score = 0;
    for (int y = 0; y < BOARD_HEIGHT_TOTAL; y++) {
        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
            data->board[y][x] = -1;
        }
    }
}

char *get_rand_header() {
    int rand_value = (rand() % (n_headers));
    return headers[rand_value];
}

void menu_option(menuptr wmenu, gameptr wgame){
    switch (wmenu->highlight) {
        case 2:
            reset_game_data(wgame);
        case 1:
            wmenu->game_active = 1;
            wmenu->highlight = 1;
            wmenu->rand_header = pause_header;

            delwin(wmenu->win);
            wmenu->win = NULL;

            int res = game(wgame);
            if(res == 2){
                wmenu->highlight = 2;
                wmenu->game_active = 0;
                wmenu->rand_header = "Game Over";
            }
            else{
                wmenu->quit = res;
            }
            del_game_win(wgame);
            break;
        case 3:
            wmenu->quit = 1;
    }
}