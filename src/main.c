#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#include "common.h"
#include "game.h"
#include "menu.h"

#define GRASS_PAIR     1
#define WATER_PAIR     2
#define MOUNTAIN_PAIR  3
#define PLAYER_PAIR    4

char *choices[] = {
        "Resume",
        "New Game",
        "Quit",
};
int n_choices = sizeof(choices) / sizeof(char *);

void draw_menu();

int menu(int game_active);

int game();

int main() {
//    setlocale(LC_ALL, "");
//    initscr();
//    keypad(stdscr, TRUE);
//    curs_set(0);
//    noecho();
//    start_color();
//    init_pair(GRASS_PAIR, COLOR_YELLOW, COLOR_GREEN);
//    init_pair(WATER_PAIR, COLOR_CYAN, COLOR_BLUE);
//    init_pair(MOUNTAIN_PAIR, COLOR_BLUE, COLOR_WHITE);
//    init_pair(PLAYER_PAIR, COLOR_RED, COLOR_MAGENTA);

    test();
//    game();
//    menu(0);
//
//    clear();
//    addstr("press any key to exit...");
//    getch();
//    endwin();
//    return 0;
}

void draw_menu(int highlight, int game_active) {
    int x, y, i;
    x = 2;
    y = 2;
    box(stdscr, 0, 0);
    for (i = 1; i <= n_choices; ++i) {
        if(!game_active && i == 1){
            attron(A_DIM);
            mvwprintw(stdscr, y, x, "%s", choices[i-1]);
            attroff(A_DIM);

        }
        else if (highlight == i)
        {
            attron(A_REVERSE);
            mvwprintw(stdscr, y, x, "%s", choices[i-1]);
            attroff(A_REVERSE);
        } else
            mvwprintw(stdscr, y, x, "%s", choices[i-1]);
        ++y;
    }
//    wrefresh(stdscr);
    refresh();
}

int menu_option(int option) {
    switch (option) {
        case 1:
            return 0;
        case 2:
            return 1;
        case 3:
            endwin();
            exit(0);
        default:
            return 0;
    }
}

int menu(int game_active) {
    int highlight = game_active ? 1 : 2;
    int new_game, c;
    draw_menu(highlight, game_active);
    while ((c = getch()) != 'q') {
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
                if (menu_option(highlight)) {
                    if (game_active) return 1;
                    do { new_game = game(); } while (new_game);
                } else {
                    return 0;
                }
            default:
                continue;
        }
        draw_menu(highlight, game_active);
    }
    getchar();
    return 0;
}

int game() {
//    int game_x = 10, game_y = 20;
    int colors[10] = {COLOR_BLUE, COLOR_CYAN, COLOR_MAGENTA, COLOR_MAGENTA, COLOR_YELLOW};
    int n_colors = 4;
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
                if (menu(1)) return 1;
                break;
            default:
                continue;
        }
//        clear();
        counter++;
        attron(COLOR_PAIR(counter % n_colors + 1));
        mvwprintw(stdscr, y, x, "\xe2\x96\x88");
        mvwprintw(stdscr, y, x + 1, "\xe2\x96\x88");
        attroff(COLOR_PAIR(counter % n_colors + 1));
        refresh();
    }
    return 0;
}