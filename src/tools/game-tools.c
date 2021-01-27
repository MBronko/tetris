#include "ncurses.h"
#include <stdlib.h>

#include "common.h"
#include "../draw/draw-game.h"
#include "game-tools.h"
#include "../components/game.h"

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

void get_new_block(gameptr game_data){
    int new_next;
    do{
        new_next = get_rand_block_num();
    } while (game_data->next_block == new_next || game_data->act_block.color == new_next);

    if(game_data->next_block == -1) {
        game_data->act_block = get_block(get_rand_block_num());
    }
    else{
        game_data->act_block = get_block(game_data->next_block);
    }
    game_data->next_block = new_next;
    draw_next_block(game_data);

    if(!is_legal_pos(game_data)){
        quit = 3;
        jumped = 1;
        gameover_view(game_data);
    }
}

void shift_board(gameptr game_data, int line) {
    int y;
    int check = 1;
    for(y = line+1; check; y++){
        check = 0;
        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
            if(game_data->board[y][x] != -1) check = 1;
            game_data->board[y-1][x] = game_data->board[y][x];
        }
    }
//    for (int y = line+1; y < BOARD_HEIGHT_TOTAL; y++) {
//        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
//            game_data->board[y-1][x] = game_data->board[y][x];
//        }
//    }
}

void remove_lines(gameptr game_data){
    int n = game_data->act_block.n;
    int off_y = game_data->act_block.y;
    int score = 0, counter = 0;
    int max_y = 0, actual_max_y = game_data->used_lines;
    for (int y = off_y + n; y >= off_y; y--) {
        if(y<0) continue;
        if(y > max_y) max_y = y;
        int check = 1;
        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
            if(game_data->board[y][x] == -1) check = 0;
        }
        if(check){
            shift_board(game_data, y);
            counter++;
            score += counter;
            game_data->used_lines--;
        }
    }
    game_data->used_lines = actual_max_y < max_y ? max_y : actual_max_y;
    game_data->used_lines-=counter;
    game_data->score += score*100;
    draw_scoreboard(game_data);
}

void place_block(gameptr game_data){
    int n = game_data->act_block.n;
    int off_x = game_data->act_block.x;
    int off_y = game_data->act_block.y;
    int rotation = game_data->act_block.rotation;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if(game_data->act_block.board[process_rotation(x,y,n,rotation)]){
                game_data->board[off_y+y][off_x+x] = game_data->act_block.color;
            }
        }
    }
    remove_lines(game_data);

    get_new_block(game_data);
}