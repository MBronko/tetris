#include <ctype.h>
#include "ncurses.h"
#include <time.h>

#include "game.h"
#include "draw.h"

int game(gameptr wgame) {
    short colors[] = {COLOR_BLUE, COLOR_RED, COLOR_WHITE, COLOR_YELLOW, COLOR_CYAN};
    int n_colors = sizeof colors / sizeof colors[0];
    int c, x = 20, y = 10, pause = 0;
    int counter = 0;
    timeout(TICK_TIME_MS);

    clear();
    game_resize(wgame);
    while (pause == 0) {
        while (!pause && (c = getch()) != ERR) {
            if (c == KEY_RESIZE) {
                game_resize(wgame);
                continue;
            }
            if (wgame->win_board == NULL) continue;
            switch (tolower(c)) {
                case ' ':
                    y -= 2;
                    break;
                case KEY_UP:
                case 'w':
                    y--;
                    break;
                case KEY_LEFT:
                case 'a':
                    x -= 2;
                    break;
                case KEY_DOWN:
                case 's':
                    y++;
                    break;
                case KEY_RIGHT:
                case 'd':
                    x += 2;
                    break;
                case 'q':
                    pause = 2;
                    continue;
                case 'p':
                    pause = 1;
                    continue;
                default:
                    continue;
            }
            attron(COLOR_PAIR(colors[counter % n_colors]));
            mvprintw(y, x, "\xe2\x96\x88");
            mvprintw(y, x + 1, "\xe2\x96\x88");
            attroff(COLOR_PAIR(colors[counter % n_colors]));
            refresh();
            counter++;
        }
    }
    timeout(-1);
    return pause - 1;
}