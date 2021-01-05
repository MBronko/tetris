#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

int highlight = 2, game_active = 0;
char *rand_header;
WINDOW * wmenu;

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
    int rand_value = (rand() % (n_headers));
    return headers[rand_value];
}

void menu() {
    rand_header = get_rand_header();
    int c, quit = 0;

//    viewptr wgame = calloc(1, sizeof(game_view));
//    wgame->board = newwin(BOARD_HEIGHT_VISIBLE, BOARD_WIDTH, 0, 0);
//    wgame->next_block = newwin(BLOCK_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, 0);
//    wgame->legend = newwin(LEGEND_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, 0);
//    wgame->scoreboard = newwin(SCORE_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, 0);

    wmenu = newwin(MENU_HEIGHT, MENU_WIDTH, 0, 0);
    menu_resize();

    dataptr data = (dataptr)calloc(1, sizeof(game_data));
    data->next_block = (blockptr)calloc(1, sizeof(block));
//    draw_game(wgame, data);
//    getch();

    draw_menu();
    while (!quit && (c = getch()) != 'q') {
        if (c == KEY_RESIZE) {
            menu_resize();
            continue;
        }
        if (wmenu == NULL) continue;
        switch (tolower(c)) {
            case KEY_UP:
            case 'w':
                if ((game_active && highlight != 1) ||
                    (!game_active && highlight != 2))
                    highlight--;
                break;
            case KEY_DOWN:
            case 's':
                if (highlight != n_options) highlight++;
                break;
            case 10:
                switch (highlight) {
                    case 2:
                        reset_game_data(data);
                    case 1:
                        game_active = 1;
                        highlight = 1;
//                        rand_header = pause_header;
                        rand_header = get_rand_header();
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
        draw_menu();
    }
    reset_game_data(data);
    free(data->next_block);
    free(data);
}
