#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <ncurses.h>

#include "common.h"
#include "menu.h"
#include "game.h"
#include "draw.h"

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

void menu() {
    int highlight = 2, game_active = 0, quit = 0;
    int c, win_too_small = 0;


    viewptr wgame = calloc(1, sizeof(game_view));
    wgame->board = newwin(BOARD_HEIGHT_VISIBLE, BOARD_WIDTH, 0, 0);
    wgame->next_block = newwin(BLOCK_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, 0);
    wgame->legend = newwin(LEGEND_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, 0);
    wgame->scoreboard = newwin(SCORE_WINDOW_HEIGHT, SIDE_PANEL_WIDTH, 0, 0);

    WINDOW *wmenu = newwin(MENU_HEIGHT,MENU_WIDTH,0,0);
    wmenu = menu_resize(wmenu, highlight, game_active);

    dataptr data = calloc(1, sizeof(game_data));
    data->next_block = calloc(1, sizeof(block));
//    draw_game(wgame, data);
//    getch();

    int n_choices = draw_menu(wmenu, highlight, game_active);
    while (!quit && (c = getch()) != 'q') {
        if(c == KEY_RESIZE){
            wmenu = menu_resize(wmenu, highlight, game_active);
            continue;
        }
        if(wmenu == NULL) continue;
        switch (tolower(c)) {
            case KEY_UP:
            case 'w':
                if ((game_active && highlight != 1) || (!game_active && highlight != 2)) highlight--;
                break;
            case KEY_DOWN:
            case 's':
                if (highlight != n_choices) highlight++;
                break;
            case 10:
                switch (highlight) {
                    case 2:
                        reset_game_data(data);
                    case 1:
                        game_active = 1;
                        highlight = 1;
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
//        wclear(wmenu);
        draw_menu(wmenu, highlight, game_active);
    }
    reset_game_data(data);
    free(data->next_block);
    free(data);
}
