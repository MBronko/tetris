#include <stdio.h>

#define _GNU_SOURCE

#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define GRASS_PAIR     1
//#define EMPTY_PAIR     1
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

void menu();

void game();

int main() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    start_color();
    init_pair(GRASS_PAIR, COLOR_YELLOW, COLOR_GREEN);
    init_pair(WATER_PAIR, COLOR_CYAN, COLOR_BLUE);
    init_pair(MOUNTAIN_PAIR, COLOR_BLUE, COLOR_WHITE);
    init_pair(PLAYER_PAIR, COLOR_RED, COLOR_MAGENTA);

//    menu();
    game();

    clear();
    addstr("press any key to exit...");
    getch();
    endwin();
    return 0;
}

//void draw_menu(int highlight) {
//    int x, y, i;
//    x = 2;
//    y = 2;
//    box(stdscr, 0, 0);
//    for (i = 0; i < n_choices; ++i) {
//        if (highlight == i + 1) /* High light the present choice */
//        {
//            wattron(stdscr, A_REVERSE);
//            mvwprintw(stdscr, y, x, "%s", choices[i]);
//            wattroff(stdscr, A_REVERSE);
//        } else
//            mvwprintw(stdscr, y, x, "%s", choices[i]);
//        ++y;
//    }
////    wrefresh(stdscr);
//    refresh();
//}
//
//void menu() {
//    draw_menu(1);
//    getchar();
////    game();
//}

void game() {
//    int game_x = 10, game_y = 20;
    int c, x = 20, y = 10;
    int counter = 0;
    while ((c = getch()) != 'q') {
        switch (c) {
            case ' ':
                y -= 2;
                break;
            case KEY_UP:
            case 'w':
                y--;
                break;
            case KEY_LEFT:
            case 'a':
                x--;
                break;
            case KEY_DOWN:
            case 's':
                y++;
                break;
            case KEY_RIGHT:
            case 'd':
                x++;
                break;
            default:
                continue;
        }
        clear();
        counter++;
        attron(COLOR_PAIR(counter % 4 + 1));
        mvwprintw(stdscr, y, x, "\xe2\x96\x88");
        attroff(COLOR_PAIR(counter % 4 + 1));
        refresh();
    }
}