#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <ncurses.h>

#include "../tools/common.h"
#include "menu.h"
#include "../tools/menu-tools.h"
#include "../draw/draw-menu.h"

char pause_header[] = "Pause";

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

    int ch;
    while (wmenu->quit != GAME_STATE_QUIT && (ch = getch()) != 'q') {
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
            case KEY_ENTER:
            case KEY_ENTER_2:
                menu_option(wmenu, wgame);
                menu_resize(wmenu);
            default:
                continue;
        }
        draw_menu(wmenu);
    }

    free(wgame);
    free(wmenu);
}