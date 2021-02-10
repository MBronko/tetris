#include "ncurses.h"
#include <stdlib.h>

#include "common.h"
#include "game-tools.h"
#include "../draw/draw-game.h"
#include "../components/game.h"


int get_rand_block_num(){
    return rand() % global_block_count;
}

block get_block(int n){
    block new = global_blocks[n];
    new.rotation = 0;
    new.x = (BOARD_GAME_WIDTH-new.n)/2;
    new.y = BOARD_GAME_HEIGHT;
//    if new block is 4 units wide then shift it to the left
    if (new.n == 4) new.y--;
    return new;
}

void get_new_block(gameptr game_data){
    int new_next;
//    get random new next block
    do{
        new_next = get_rand_block_num();
    } while (game_data->next_block == new_next || game_data->act_block.color == new_next);

//    get new actual block (from next_block variable or random if theres none)
    if(game_data->next_block == -1) {
        game_data->act_block = get_block(get_rand_block_num());
    }
    else{
        game_data->act_block = get_block(game_data->next_block);
    }
    game_data->next_block = new_next;
    draw_next_block(game_data);

//    end the game if new block cant be placed without collision
    if(!is_legal_pos(game_data)){
        global_game_state = GAME_STATE_GAMEOVER;
        global_jumped = true;
        gameover_view(game_data);
    }
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

//    get x, y and return index of one dimensional block array after processing rotation
//              indexes of array where
//    n=3 and rotation=0    n=3 and rotation=1
//          6 7 8                 0 3 6
//          3 4 5                 1 4 7
//          0 1 2                 2 5 8
//    so this function for x=0, y=0, n=3
//    when rotation=0 returns 0
//    when rotation=1 returns 2
int process_rotation(int x, int y, int n, int rotation){
//   https://youtu.be/8OK8_tHeCIA?t=362  formulas explanation
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
//    only rotated blocks right now are the ones with size of 3 or 4
//    might be changed in case of adding new blocks of different sizes
    return y*n+x;
}

//    shift every higher row down
//          1          0
//    like  2    to    1   when line=2 (line 2 is lost)
//          3          3
//    but stop on first empty line
void shift_board(gameptr game_data, int line) {
    int y;
    bool check = true;
    for(y = line+1; check; y++){
        check = false;
        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
            if(game_data->board[y][x] != -1) check = true;
            game_data->board[y-1][x] = game_data->board[y][x];
        }
    }
}

//    look for full lines near actual block
bool remove_lines(gameptr game_data){
    int n = game_data->act_block.n;
    int off_y = game_data->act_block.y;
    int score = 0, counter = 0;
    for (int y = off_y + n; y >= off_y; y--) {
        if(y<0) break;
        int check = 1;
        for (int x = 0; x < BOARD_GAME_WIDTH; x++) {
            if(game_data->board[y][x] == -1) check = 0;
        }
        if(check){
            shift_board(game_data, y);
            counter++;
            score += counter;
        }
    }
    game_data->score += score*100;
    draw_scoreboard(game_data);
    return counter > 0;
}

//    copy actual block to the board
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
}