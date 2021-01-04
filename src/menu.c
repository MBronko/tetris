#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <ncurses.h>

#include "common.h"
#include "menu.h"
#include "game.h"
#include "draw.h"

void reset_game_data(game_data *data) {
    data->score = 0;
    for (int x = 0; x < BOARD_HEIGHT; x++) {
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
    int c;

    char *choices[] = {
            "Resume",
            "New Game",
            "Quit",
    };
    int n_choices = sizeof(choices) / sizeof(choices[0]);

    draw_menu(stdscr, highlight, game_active, choices, n_choices);

    game_data *data = calloc(1, sizeof(game_data));
    data->next_block = calloc(1, sizeof(block));

    while (!quit && (c = getch()) != 'q') {
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
                        game(data);
                        break;
                    case 3:
                        quit = 1;
                        continue;
                    default:
                        continue;
                }
                break;
//                if (menu_option(highlight)) {
//                    if (game_active) return 1;
//                    do { new_game = game(); } while (new_game);
//                } else {
//                    return 0;
//                }
            default:
                continue;
        }
        draw_menu(stdscr, highlight, game_active, choices, n_choices);
    }
    reset_game_data(data);
    free(data->next_block);
    free(data);
}
