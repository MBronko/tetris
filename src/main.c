#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include "components/menu.h"

int main() {
//    ncurses setup
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    if(has_colors()) start_color();

    for(short i=0; i<16; i++){
        init_pair(i, i, i);
    }
//    end of ncurses setup

    srand(time(NULL));
    menu();

//    restore terminal after running ncurses
    endwin();
    return 0;
}