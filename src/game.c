#include <ctype.h>
#include "ncurses.h"

#include "game.h"
#include "draw.h"

int game() {
//    int game_x = 10, game_y = 20;
    short colors[] = {COLOR_BLUE, COLOR_RED, COLOR_WHITE, COLOR_YELLOW, COLOR_CYAN};
    int n_colors = sizeof colors / sizeof colors[0];
    int c, x = 20, y = 10;
    int counter = 0;
    clear();
    while ((c = getch()) != 'q') {
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
            case 'p':
                return 0;
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
    return 0;
}