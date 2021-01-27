#include <ctype.h>
#include "ncurses.h"
#include <time.h>
#include <pthread.h>

#include "game.h"
#include "../draw/draw-game.h"
#include "../tools/game-tools.h"

int jumped = 0, quit = 0;
//    dane z klockami
short colors[] = {COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_YELLOW, COLOR_GREEN, COLOR_MAGENTA, COLOR_RED};
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

int gravity(gameptr game_data) {
    game_data->act_block.y--;
    if(!is_legal_pos(game_data)){
        game_data->act_block.y++;

        place_block(game_data);
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
    time.tv_nsec = (MINI_TICK_TIME_MS)*1000000L; // 1ms = 1.000.000ns

    while(!quit){
        nanosleep(&time, NULL);
        count++;
        if(jumped){
           count = 0;
            jumped = 0;
        }
        if(count >= TICK_TIME_MS/MINI_TICK_TIME_MS){
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
        get_new_block(game_data);
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