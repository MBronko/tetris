#include <ctype.h>
#include "ncurses.h"
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

#include "game.h"
#include "../draw/draw-game.h"
#include "../tools/game-tools.h"

bool global_jumped = false;
int global_game_state = GAME_STATE_ONGOING;

const short global_colors[] = {COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_YELLOW, COLOR_GREEN, COLOR_MAGENTA, COLOR_RED};
const int global_block_count = 7;

block global_blocks[7] = {
        {{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, 4, 0},  // i
        {{0, 0, 0, 1, 1, 1, 1, 0, 0}, 3, 1},  // j
        {{0, 0, 0, 1, 1, 1, 0, 0, 1}, 3, 2},  // l
        {{1, 1, 1, 1}, 2, 3},  // o
        {{1, 1, 0, 0, 1, 1, 0, 0, 0}, 3, 4},  // s
        {{0, 0, 0, 1, 1, 1, 0, 1, 0}, 3, 5},  // t
        {{0, 1, 1, 1, 1, 0, 0, 0, 0}, 3, 6}   // z
};
//       {{map}, square size, color}
//       6, 7, 8       3
//       3, 4, 5
//       0, 1, 2
//       color number should be the same as array index

void gravity(gameptr game_data) {
    game_data->act_block.y--;
    bool draw_whole_board = false;

    int legal_pos = is_legal_pos(game_data);
    if(!legal_pos){
        game_data->act_block.y++;

        place_block(game_data);

//        remove_lines(game_data);
        draw_whole_board = remove_lines(game_data);

        get_new_block(game_data);
    }

    if (global_game_state == GAME_STATE_ONGOING){
//        draw whole board only when lines are removed
        draw_board(game_data, draw_whole_board);
    }
}

void move_block(gameptr game_data, int dir){
    game_data->act_block.x += dir;
    if(is_legal_pos(game_data)) draw_board(game_data, false);
    else game_data->act_block.x -= dir;
}

void rotate(gameptr game_data, int dir) {
    game_data->act_block.rotation += dir;
    if(game_data->act_block.rotation < 0) game_data->act_block.rotation = 3;
    if(game_data->act_block.rotation > 3) game_data->act_block.rotation = 0;

    if(is_legal_pos(game_data)){
        draw_board(game_data, false);
    }
    else{
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
    time.tv_nsec = (MINI_TICK_TIME_MS)*1000000L; // 1ms = 1.000.000ns

    while(global_game_state == GAME_STATE_ONGOING){
        nanosleep(&time, NULL);
        count++;
//        if user used gravity then reset timer
        if(global_jumped){
            count = 0;
            global_jumped = false;
        }
        if(count >= TICK_TIME_MS/MINI_TICK_TIME_MS){
            gravity(game_data);
            count = 0;
        }
    }

    return NULL;
}

int game(gameptr game_data) {
    bool skip_gameover = false;
    global_jumped = false;
    global_game_state = GAME_STATE_ONGOING;
    int c;
    pthread_t thread;

//    if it's new game then get new blocks
    if(game_data->next_block == -1){
        get_new_block(game_data);
    }

    clear();
    game_resize(game_data);
    pthread_create(&thread, NULL, gravity_loop, game_data);

    while (global_game_state == GAME_STATE_ONGOING && (c = getch()) != ERR) {
        if (c == KEY_RESIZE) {
            game_resize(game_data);
            continue;
        }
        if (game_data->win_board == NULL) continue;
        switch (tolower(c)) {
            case KEY_DOWN:
            case 's':
                global_jumped = true;
                gravity(game_data);
                break;
            case KEY_UP:
            case 'x':
                rotate(game_data, DIRECTION_RIGHT);
                break;
            case KEY_LEFT:
            case 'a':
                move_block(game_data, DIRECTION_LEFT);
                break;
            case KEY_RIGHT:
            case 'd':
                move_block(game_data, DIRECTION_RIGHT);
                break;
            case 'z':
                rotate(game_data, DIRECTION_LEFT);
                break;
            case KEY_ENTER:
            case KEY_ENTER_2:
//                game over view sometimes was stuck on last iteration of this loop
//                so if user exits view within this loop then skip the second loop
                if(global_game_state == GAME_STATE_GAMEOVER) skip_gameover = true;
                continue;
            case 'q':
                global_game_state = GAME_STATE_QUIT;
                skip_gameover = true;
                continue;
            case 'p':
                global_game_state = GAME_STATE_PAUSE;
                skip_gameover = true;
                continue;
            default:
                continue;
        }
    }
    while(global_game_state == GAME_STATE_GAMEOVER && !skip_gameover && (c = getch())){
        if(c == KEY_ENTER || c == KEY_ENTER_2) break;
        if(c == 'q'){
            global_game_state = GAME_STATE_QUIT;
            break;
        }
    }

//    wait for loop to end to avoid loops stacking
    pthread_join(thread, NULL);

    return global_game_state;
}