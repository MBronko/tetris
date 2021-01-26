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
        {{0, 0, 0, 1, 1, 1, 1, 0, 0}, 3, 1},  // j
        {{0, 0, 0, 1, 1, 1, 0, 0, 1}, 3, 2},  // l
        {{1, 1, 1, 1}, 2, 3},  // o
        {{1, 1, 0, 0, 1, 1, 0, 0, 0}, 3, 4},  // s
        {{0, 0, 0, 1, 1, 1, 0, 1, 0}, 3, 5},  // t
        {{0, 1, 1, 1, 1, 0, 0, 0, 0}, 3, 6}   // z
};

int get_rand_block_num(){
    return rand() % block_count;
}

block get_block(int n){
    block new = blocks[n];
    new.rotation = 0;
    new.x = (BOARD_GAME_WIDTH-new.n)/2;
    new.y = BOARD_GAME_HEIGHT;
    if (new.n == 4) new.y--;
    return new;
}

block get_rand_block(){
    return get_block(get_rand_block_num());
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
    else if(n == 4){
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
    else{
        return y*n+x;
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
            if((off_x + x < 0 || off_x + x >= BOARD_GAME_WIDTH) && game_data->act_block.board[process_rotation(x,y,n,rotation)]) return 0;
            if(game_data->act_block.board[process_rotation(x, y, n, rotation)]){
                if(game_data->board[off_y+y][off_x+x] != -1 || off_y+y < 0){
                    return 0;
                }
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
    int n = game_data->act_block.n;
    int off_x = game_data->act_block.x;
    int off_y = game_data->act_block.y;
    int rotation = game_data->act_block.rotation;
    game_data->act_block.y--;
    if(!is_legal_pos(game_data)){
        game_data->act_block.y++;

        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                if(game_data->act_block.board[process_rotation(x,y,n,rotation)]){
                    game_data->board[off_y+y][off_x+x] = game_data->act_block.color;
                }
            }
        }
        game_data->act_block = get_block(game_data->next_block);
        game_data->next_block = get_rand_block_num();
        draw_next_block(game_data);

        int score = 0, counter = 1;
        for (int y = off_y; y <= off_y+n; y++) {
            if(y<0) continue;
            int check = 1;
            for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
                if(game_data->board[y][x] == -1) check = 0;
            }
            if(check){
                shift_board(game_data, y);
                score += counter;
                counter++;
                y--;
            }
        }
        game_data->score += score*100;
        draw_scoreboard(game_data);
    }

//    sprawdz czy game_over po ruchu, zwróć 0, może header z game over

    return 1;
}

void move_block(gameptr game_data, int dir){
    game_data->act_block.x += dir;
    if(!is_legal_pos(game_data)) game_data->act_block.x -= dir;
}

void rotate(gameptr game_data, int dir) {
    game_data->act_block.rotation += dir;
    if(game_data->act_block.rotation < 0) game_data->act_block.rotation = 3;
    if(game_data->act_block.rotation > 3) game_data->act_block.rotation = 0;

    if(!is_legal_pos(game_data)){
        game_data->act_block.rotation -= dir;
        if(game_data->act_block.rotation < 0) game_data->act_block.rotation = 3;
        if(game_data->act_block.rotation > 3) game_data->act_block.rotation = 0;
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
            jumped = 0;
        }
        if(count >= 7){
            gravity(game_data);
            draw_board(game_data);
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

    jumped = quit = 0;
    int c;
    pthread_t thread;

    clear();
    game_resize(game_data);
    pthread_create(&thread, NULL, gravity_loop, game_data);
    while (!quit && (c = getch()) != ERR) {
        if (c == KEY_RESIZE) {
            game_resize(game_data);
            continue;
        }
        if (game_data->win_board == NULL) continue;
        switch (tolower(c)) {
            case ' ':
                jumped = 1;
                gravity(game_data);
                draw_board(game_data);
                break;
            case KEY_UP:
            case 'w':
                rotate(game_data, 1);
                draw_board(game_data);
                break;
            case KEY_LEFT:
            case 'a':
                move_block(game_data, -1);
                draw_board(game_data);
                break;
            case KEY_DOWN:
            case 's':
                rotate(game_data, -1);
                draw_board(game_data);
                break;
            case KEY_RIGHT:
            case 'd':
                move_block(game_data, 1);
                draw_board(game_data);
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
    }
    return quit - 1;
}