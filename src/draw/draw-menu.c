#include <ncurses.h>
#include <string.h>

#include "../tools/common.h"
#include "../tools/draw-tools.h"
#include "../components/menu.h"
#include "draw-menu.h"

void draw_menu(menuptr wmenu){
    int y = 2;
    box(wmenu->win, 0, 0);

    wattron(wmenu->win, A_BOLD);
    mvwprintw(wmenu->win, y, center_text(wmenu->win, wmenu->rand_header), "%s", wmenu->rand_header);
    wattroff(wmenu->win, A_BOLD);
    y+=2;

    for (int i = 0; i < global_n_options; ++i) {
        int x = center_text(wmenu->win, global_options[i]);
        if (!wmenu->game_active && i == 0) {
            wattron(wmenu->win, A_DIM);
            mvwprintw(wmenu->win, y, x, "%s", global_options[i]);
            wattroff(wmenu->win, A_DIM);

        } else if (wmenu->highlight == i + 1) {
            wattron(wmenu->win, A_REVERSE);
            mvwprintw(wmenu->win, y, x, "%s", global_options[i]);
            wattroff(wmenu->win, A_REVERSE);
        } else
            mvwprintw(wmenu->win, y, x, "%s", global_options[i]);
        y++;
    }
    clear();
    refresh();
    wrefresh(wmenu->win);
}

void menu_resize(menuptr wmenu){
    if(check_terminal_size()){
        int new_y = center_y(MENU_HEIGHT);
        int new_x = center_x(MENU_WIDTH);
        if(wmenu->win != NULL){
            if(wmenu->win_x != new_x || wmenu->win_y != new_y) {
                mvwin(wmenu->win, new_y, new_x);
            }
        }
        else{
            wmenu->win = newwin(MENU_HEIGHT,MENU_WIDTH,new_y,new_x);
        }
        wmenu->win_x = new_x;
        wmenu->win_y = new_y;
        draw_menu(wmenu);
    }
    else if(wmenu->win != NULL){
        delwin(wmenu->win);
        wmenu->win = NULL;
    }
}
