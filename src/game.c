#include <ctype.h>
#include "ncurses.h"
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#include "game.h"
#include "draw.h"

int jumped = 0, quit = 0;
//    dane z klockami
short colors[] = {COLOR_BLUE, COLOR_RED, COLOR_WHITE, COLOR_YELLOW, COLOR_CYAN, COLOR_MAGENTA, COLOR_GREEN};
int block_count = 7;
block blocks[7] = {
        {{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, 4, 0},  // i
        {{1, 1, 1, 1, 0, 0, 0, 0, 0}, 3, 1},  // j
        {{1, 1, 1, 0, 0, 1, 0, 0, 0}, 3, 2},  // l
        {{1, 1, 1, 1}, 2, 3},  // o
        {{1, 1, 0, 0, 1, 1, 0, 0, 0}, 3, 4},  // s
        {{1, 1, 1, 0, 1, 0, 0, 0, 0}, 3, 5},  // t
        {{0, 1, 1, 1, 1, 0, 0, 0, 0}, 3, 6}   // z
};

int get_rand_block_num(){
    return rand() % block_count;
}

block get_rand_block(){
    block new = blocks[get_rand_block_num()];
    new.rotation = 0;
    new.x = 2;
    new.y = 2;
    return new;
}

//   https://youtu.be/8OK8_tHeCIA?t=362  formulas explanation
int process_rotation(int x, int y, int n, int rotation){
    if(n == 3){
        switch (rotation) {
            case 0:
                return y*n+x;
            case 1:
                return 2-y+x*n;
            case 2:
                return 8-y*n-x;
            case 3:
                return 6+y-x*n;
        }
    }
    else{
        switch (rotation) {
            case 0:
                return y*n+x;
            case 1:
                return 3-y+x*n;
            case 2:
                return 15-y*n-x;
            case 3:
                return 12+y-x*n;
        }
    }
    return 0; // shouldnt happen
}

int is_legal_pos(gameptr game_data) {
    int off_x = game_data->act_block.x;
    int off_y = game_data->act_block.y;
    int n = game_data->act_block.n;
    int rotation = game_data->act_block.rotation;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if(off_y+y < 0 || off_x+x < 0) continue;
            if(game_data->board[off_y+y][off_x+x] != -1 && game_data->act_block.board[process_rotation(x, y, n, rotation)]){
                return 0;
            }
        }
    }
    return 1;
}

void shift_board(gameptr game_data, int line) {
    for (int y = line+1; y < BOARD_HEIGHT_TOTAL; y++) {
        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
            game_data->board[y-1][x] = game_data->board[y][x];
        }
    }
    for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
        game_data->board[BOARD_HEIGHT_TOTAL-1][x] = -1;
    }
}

int gravity(gameptr game_data) {
//    sprawdź czy jest to możliwy ruch
//    jeżeli nie to zaktualizuj tablice o klocka,
//    sprawdź zapełnione linie, dodaj score i odśwież,
//    zaktualizuj aktualny klocek i wylosuj nowy do next

//    sprawdz czy game_over po ruchu, zwróć 0, może header z game over

//    narysuj board od nowa
    return 1;
}

void move_block(gameptr game_data, int dir){
    if(game_data->act_block.x + dir < 0 || game_data->act_block.x + dir >= BOARD_GAME_WIDTH) return;

    game_data->act_block.x += dir;
    if(!is_legal_pos(game_data)) game_data->act_block.x -= dir;
}

void rotate(gameptr game_data, int dir) {
    game_data->act_block.rotation += dir;
    game_data->act_block.rotation = game_data->act_block.rotation % 4;

    if(!is_legal_pos(game_data)){
        game_data->act_block.rotation -= dir;
        game_data->act_block.rotation = game_data->act_block.rotation % 4;
    }
}

void *gravity_loop(void *value){
    gameptr game_data = (gameptr)value;
    int count = 0;

    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = 100000000L; // 100ms / 100.000.000 ns

    while(!quit){
        nanosleep(&time, NULL);
        count++;
        if(jumped){
           count = 0;
        }
        if(count >= 10){
            gravity(game_data);
            jumped = 0;
            count = 0;
        }
    }

    return NULL;
}

int game(gameptr game_data) {
    clear();
    refresh();
    for (int k = 0; k<7; k++){
        for (int i = 0; i < blocks[k].n; i++) {
            for (int j = 0; j < blocks[k].n; j++) {
                if (blocks[k].board[blocks[k].n*i+j]) mvprintw(blocks[k].n - i - 1, j+5*k, "*");
            }
        }
    }
    if(game_data->next_block == -1){
        game_data->act_block = get_rand_block();
        game_data->next_block = get_rand_block_num();
    }

//    int n = game_data->act_block.n;
//    for (int y = 0; y < wgame->act_block.n; y++) {
//        for (int x = 0; x < wgame->act_block.n; x++) {
//            if (wgame->act_block.board[n*y+x]) mvprintw(wgame->act_block.n - y - 1, x, "*");
//        }
//    }

//    getch();
    jumped = quit = 0;

//    int n_colors = sizeof colors / sizeof colors[0];
//    int c, x = 20, y = 10, pause = 0;
    int counter = 0;
//    timeout(TICK_TIME_MS);
    int c;

    clear();
    game_resize(game_data);
    while (!quit && (c = getch()) != ERR) {
        if (c == KEY_RESIZE) {
            game_resize(game_data);
            continue;
        }
        if (game_data->win_board == NULL) continue;
        switch (tolower(c)) {
            case ' ':
                break;
            case KEY_UP:
            case 'w':
                break;
            case KEY_LEFT:
            case 'a':
                break;
            case KEY_DOWN:
            case 's':
                break;
            case KEY_RIGHT:
            case 'd':
                break;
            case 'q':
                quit = 2;
                continue;
            case 'p':
                quit = 1;
                continue;
            default:
                continue;
        }
        draw_game(game_data);
//            attron(COLOR_PAIR(colors[counter % n_colors]));
//            mvprintw(y, x, "\xe2\x96\x88");
//            mvprintw(y, x + 1, "\xe2\x96\x88");
//            attroff(COLOR_PAIR(colors[counter % n_colors]));
        refresh();
        counter++;
    }
    return quit - 1;
}