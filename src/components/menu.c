#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <ncurses.h>

#include "../tools/common.h"
#include "menu.h"
#include "../tools/menu-tools.h"
#include "../draw/draw-menu.h"

char global_pause_header[] = "Pause";
char global_gameover_header[] = "Game Over";

//    random generated headers (like minecraft xD)
char *global_headers[] = {
        "Hello!!",
        "Let's Play Tetris!",
        "Siemanko",
        "No hejka",
        "Gitara siema"
};
int global_n_headers = sizeof(global_headers) / sizeof(global_headers[0]);

char *global_options[] = {
        "Resume",
        "New Game",
        "Quit",
};
int global_n_options = sizeof(global_options) / sizeof(global_options[0]);

void menu() {
    menuptr menu_data = (menuptr)calloc(1, sizeof(menu_data_str));
    menu_data->win = NULL;
    menu_data->rand_header = get_rand_header();
    menu_data->game_active = false;
//    highlight second position from top
    menu_data->highlight = 2;
    menu_data->menu_state = GAME_STATE_ONGOING;

    gameptr game_data = (gameptr)calloc(1, sizeof(game_data_str));
    game_data->win_board = NULL;
    game_data->win_next = NULL;
    game_data->win_legend = NULL;
    game_data->win_score = NULL;
    menu_resize(menu_data);

    int c;
    while (menu_data->menu_state != GAME_STATE_QUIT && (c = getch()) != 'q') {
        if (c == KEY_RESIZE) {
            menu_resize(menu_data);
            continue;
        }
        if (menu_data->win == NULL) continue;
        switch (tolower(c)) {
            case KEY_UP:
            case 'w':
//                if game is inactive (0) then minimal highlight is 2
//                if game is active (1) then minimal highlight is 1
                if (menu_data->highlight + (int)menu_data->game_active > 2) {
                    menu_data->highlight--;
                }
                break;
            case KEY_DOWN:
            case 's':
                if (menu_data->highlight < global_n_options) menu_data->highlight++;
                break;
            case KEY_ENTER:
            case KEY_ENTER_2:
                menu_option(menu_data, game_data);
                menu_resize(menu_data);
            default:
                continue;
        }
        draw_menu(menu_data);
    }

    free(game_data);
    free(menu_data);
}