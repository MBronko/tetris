#include <ctype.h>
#include "ncurses.h"
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#include "game.h"
#include "draw.h"

//    dane z klockami
short colors[] = {COLOR_BLUE, COLOR_RED, COLOR_WHITE, COLOR_YELLOW, COLOR_CYAN, COLOR_MAGENTA, COLOR_GREEN};
int block_count = 7;
block blocks[7] = {
        {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 4, 0, 0, 0},  // i
        {{{1, 1, 1}, {1, 0, 0}, {0, 0, 0}}, 3, 0, 0, 0},  // j
        {{{1, 1, 1}, {0, 0, 1}, {0, 0, 0}}, 3, 0, 0, 0},  // l
        {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, 4, 0, 0, 0},  // o
        {{{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}, 3, 0, 0, 0},  // s
        {{{1, 1, 1}, {0, 1, 0}, {0, 0, 0}}, 3, 0, 0, 0},  // t
        {{{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}, 3, 0, 0, 0}   // z
};

int get_rand_block(){
    return rand() % block_count;
}

int is_legal_move(gameptr game_data) {
    return 1;
}

void shift_board(gameptr game_data, int line) {
//    wszystkie klocki powyżej którejś lini o jeden w dół
}

int gravity(gameptr game_data) {
//    sprawdź czy jest to możliwy ruch
//    sprawdź zapełnione linie, dodaj score i odśwież

//    sprawdz czy game_over po ruchu, zwróć 0, może header z game over

//    narysuj board od nowa
    return 1;
}

void rotate(gameptr game_data) {
//    sprawdź czy jest to legalne i obróć
}

int jumped, quit;
void gravity_loop(void *value){
    gameptr game_data = (gameptr)value;


}

int game(gameptr wgame) {
    clear();
    refresh();
    for (int k = 0; k<7; k++){
        for (int i = 0; i < blocks[k].n; i++) {
            for (int j = 0; j < blocks[k].n; j++) {
                if (blocks[k].board[i][j]) mvprintw(blocks[k].n - i - 1, j+5*k, "*");
            }
        }
    }
    getch();
    jumped = quit = 0;

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
//                draw_game(wgame);
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