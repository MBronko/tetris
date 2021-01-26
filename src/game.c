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
        {{{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 0, 0, 0},  // j
        {{{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 0, 0, 0},  // l
        {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, 4, 0, 0, 0},  // o
        {{{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 0, 0, 0},  // s
        {{{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 0, 0, 0},  // t
        {{{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 3, 0, 0, 0}   // z
};

int get_rand_block(){
    return rand() % block_count;
}

int is_legal_pos(gameptr game_data) {
//    check for rotations
    int off_x = game_data->act_block.x;
    int off_y = game_data->act_block.y;
    for (int y = 0; y < BLOCK_MAX_SIZE; y++) {
        for (int x = 0; x < BLOCK_MAX_SIZE; x++) {
            if(game_data->board[off_y+y][off_x+x] != -1 && game_data->act_block.board[y][x]){
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
    if(!is_legal_pos(game_data)) game_data->act_block.x += dir;

    draw_game(game_data);
}

void rotate(gameptr game_data, int dir) {
//    sprawdź czy jest to legalne i obróć
//    dir value
//    -1 - left
//    1 - right
}

int jumped, quit;
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

int game(gameptr wgame) {
    clear();
    refresh();
//    for (int k = 0; k<7; k++){
//        for (int i = 0; i < blocks[k].n; i++) {
//            for (int j = 0; j < blocks[k].n; j++) {
//                if (blocks[k].board[i][j]) mvprintw(blocks[k].n - i - 1, j+5*k, "*");
//            }
//        }
//    }
    if(wgame->next_block == -1){
        wgame->act_block = blocks[get_rand_block()];
        wgame->next_block = get_rand_block();
    }

    for (int i = 0; i < wgame->act_block.n; i++) {
        for (int j = 0; j < wgame->act_block.n; j++) {
            if (wgame->act_block.board[i][j]) mvprintw(wgame->act_block.n - i - 1, j, "*");
        }
    }

//    getch();
    jumped = quit = 0;

//    int n_colors = sizeof colors / sizeof colors[0];
//    int c, x = 20, y = 10, pause = 0;
    int counter = 0;
//    timeout(TICK_TIME_MS);
    int pause = 0, c;

    clear();
    game_resize(wgame);
    while (!pause && (c = getch()) != ERR) {
        if (c == KEY_RESIZE) {
            game_resize(wgame);
            continue;
        }
        if (wgame->win_board == NULL) continue;
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
                pause = 2;
                continue;
            case 'p':
                pause = 1;
                continue;
            default:
                continue;
        }
//            attron(COLOR_PAIR(colors[counter % n_colors]));
//            mvprintw(y, x, "\xe2\x96\x88");
//            mvprintw(y, x + 1, "\xe2\x96\x88");
//            attroff(COLOR_PAIR(colors[counter % n_colors]));
        refresh();
        counter++;
    }
    return pause - 1;
}