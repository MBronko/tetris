#include <ncurses.h>
#include <locale.h>
#include "menu.h"

int main() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    if(has_colors()) start_color();

    for(short i=0; i<16; i++){
        init_pair(i, i, i);
    }

    menu();

    clear();
    addstr("press any key to exit...");
    getch();
    endwin();
    return 0;
}