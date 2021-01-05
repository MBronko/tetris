#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <ncurses.h>

#include "common.h"
#include "menu.h"
#include "game.h"
#include "draw.h"

char pause_header[] = "Pause";

char *headers[] = {
        "Hello!!",
        "To moje bagno!",
        "Siemanko",
        "No hejka",
        "Gitara siema"
};
int n_headers = sizeof(headers) / sizeof(headers[0]);

char *options[] = {
        "Resume",
        "New Game",
        "Quit",
};
int n_options = sizeof(options) / sizeof(options[0]);


void reset_game_data(dataptr data) {
    data->score = 0;
    for (int x = 0; x < BOARD_HEIGHT_TOTAL; x++) {
        for (int y = 0; y < BOARD_WIDTH; y++) {
            free(data->board[x][y]);
            data->board[x][y] = NULL;
        }
    }
    for (int x = 0; x < BLOCK_MAX_SIZE; x++) {
        for (int y = 0; y < BLOCK_MAX_SIZE; y++) {
            free(data->next_block->board[x][y]);
            data->next_block->board[x][y] = NULL;
        }
    }
}

char *get_rand_header() {
    int rand_header = (rand() % (n_headers));
    return headers[rand_header];
}

void menu() {
    menuptr menu_data = calloc(1, sizeof(menu_data));
    menu_data->rand_header = get_rand_header();
    menu_data->options = calloc(n_options, sizeof(char));
    for(int i=0; i<n_options; i++){
        menu_data->options[i] = options[i];
    }
//    menu_data->options = options;
    menu_data->n_options = n_options;
    menu_data->highlight = 2;
    menu_data->game_active = 0;
//    int menu_data->highlight = 2, menu_data->game_active = 0, quit = 0;
    int c, win_too_small = 0, quit = 0;


    viewptr wgame = calloc(1, sizeof(game_view));
    wgame->board = newwin(BOARD_HEIGHT_VISIBLE, BOARD_WIDTH, 0, 0);
    wgame->next_block = newwin(BLOCK_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, 0);
    wgame->legend = newwin(LEGEND_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, 0);
    wgame->scoreboard = newwin(SCORE_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, 0);

    WINDOW *wmenu = newwin(MENU_HEIGHT, MENU_WIDTH, 0, 0);
    wmenu = menu_resize(wmenu, menu_data);

    dataptr data = calloc(1, sizeof(game_data));
    data->next_block = calloc(1, sizeof(block));
//    draw_game(wgame, data);
//    getch();

    draw_menu(wmenu, menu_data);
    while (!quit && (c = getch()) != 'q') {
        if (c == KEY_RESIZE) {
            wmenu = menu_resize(wmenu, menu_data);
            continue;
        }
        if (wmenu == NULL) continue;
        switch (tolower(c)) {
            case KEY_UP:
            case 'w':
                if ((menu_data->game_active && menu_data->highlight != 1) ||
                    (!menu_data->game_active && menu_data->highlight != 2))
                    menu_data->highlight--;
                break;
            case KEY_DOWN:
            case 's':
                if (menu_data->highlight != menu_data->n_options) menu_data->highlight++;
                break;
            case 10:
                switch (menu_data->highlight) {
                    case 2:
                        reset_game_data(data);
                    case 1:
                        menu_data->game_active = 1;
                        menu_data->highlight = 1;
//                        menu_data->rand_header = pause_header;
                        menu_data->rand_header = get_rand_header();
                        game(data);
                        break;
                    case 3:
                        quit = 1;
                    default:
                        continue;
                }
                break;
            default:
                continue;
        }
        draw_menu(wmenu, menu_data);
    }
    reset_game_data(data);
    free(data->next_block);
    free(data);

//    free(menu_data->options);
    free(menu_data);
}
