#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <ncurses.h>

#include "common.h"
#include "menu.h"
#include "game.h"
#include "draw.h"

//char pause_header[] = "Pause";

char *headers[] = {
        "Hello!!",
        "To moje bagno!",
        "Siemanko",
        "No hejka",
        "Gitara siema",
        "MitFar tu był"
};
int n_headers = sizeof(headers) / sizeof(headers[0]);

char *options[] = {
        "Resume",
        "New Game",
        "Quit",
};
int n_options = sizeof(options) / sizeof(options[0]);

void reset_game_data(gameptr data) {
    data->next_block = -1;
    data->score = 0;
    for (int y = 0; y < BOARD_HEIGHT_TOTAL; y++) {
        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
//            free(data->board[x][y]);
//            data->board[x][y] = NULL;
            data->board[y][x] = 4;
        }
    }
//    for (int x = 0; x < BLOCK_MAX_SIZE; x++) {
//        for (int y = 0; y < BLOCK_MAX_SIZE; y++) {
//            free(data->next_block->board[x][y]);
//            data->next_block->board[x][y] = NULL;
//        }
//    }
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
//                        rand_header = pause_header;
            wmenu->rand_header = get_rand_header();

            delwin(wmenu->win);
            wmenu->win = NULL;

            wmenu->quit = game(wgame);
            del_game_win(wgame);
            break;
        case 3:
            wmenu->quit = 1;
    }
}

void menu() {
    menuptr wmenu = (menuptr)calloc(1, sizeof(menu_data));
    wmenu->win = NULL;
    wmenu->rand_header = get_rand_header();
    wmenu->game_active = 0;
    wmenu->highlight = 2;
    wmenu->quit = 0;

    gameptr wgame = (gameptr)calloc(1, sizeof(game_data));
    wgame->win_board = NULL;
    wgame->win_next = NULL;
    wgame->win_legend = NULL;
    wgame->win_score = NULL;

    menu_resize(wmenu);

//    dataptr data = (dataptr)calloc(1, sizeof(game_data));
//    data->next_block = (blockptr)calloc(1, sizeof(block));

    int ch;
    while (!wmenu->quit && (ch = getch()) != 'q') {
        if (ch == KEY_RESIZE) {
            menu_resize(wmenu);
            continue;
        }
        if (wmenu->win == NULL) continue;
        switch (tolower(ch)) {
            case KEY_UP:
            case 'w':
                if (wmenu->highlight + wmenu->game_active != 2) {
                    wmenu->highlight--;
                }
                break;
            case KEY_DOWN:
            case 's':
                if (wmenu->highlight != n_options) wmenu->highlight++;
                break;
            case 10:
            case KEY_ENTER:
                menu_option(wmenu, wgame);
                menu_resize(wmenu);
            default:
                continue;
        }
        draw_menu(wmenu);
    }

    reset_game_data(wgame);
    free(wgame);
//    free(data->next_block);
    free(wmenu);
}
