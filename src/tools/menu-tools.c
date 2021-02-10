#include <stdlib.h>
#include "ncurses.h"

#include "common.h"
#include "../components/menu.h"
#include "menu-tools.h"
#include "draw-tools.h"
#include "../components/game.h"

void reset_game_data(gameptr data) {
//    -1 means empty
    data->next_block = -1;
    data->score = 0;
//    board pixels are integers from -1 to 6
//      -1 means pixel is empty
//      0-6 is the color of pixel
    for (int y = 0; y < BOARD_HEIGHT_TOTAL; y++) {
        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
            data->board[y][x] = -1;
        }
    }
}

char *get_rand_header() {
    int rand_value = (rand() % (global_n_headers));
    return global_headers[rand_value];
}

void menu_option(menuptr menu_data, gameptr game_data){
    switch (menu_data->highlight) {
        case 2:
            reset_game_data(game_data);
//            fall through
        case 1:
            menu_data->game_active = true;
            menu_data->highlight = 1;
            menu_data->rand_header = global_pause_header;

            delwin(menu_data->win);
            menu_data->win = NULL;

            int game_state = game(game_data);
            if(game_state == GAME_STATE_GAMEOVER){
                menu_data->highlight = 2;
                menu_data->game_active = false;
                menu_data->rand_header = global_gameover_header;
            }
            menu_data->menu_state = game_state;

            del_game_win(game_data);
            break;
        case 3:
            menu_data->menu_state = GAME_STATE_QUIT;
    }
}