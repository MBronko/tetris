#ifndef COMMON_H
#define COMMON_H

#define BOARD_WIDTH 10

#define BOARD_HEIGHT_VISIBLE 20
#define BOARD_HEIGHT_BUF 3
#define BOARD_HEIGHT BOARD_HEIGHT_VISIBLE+BOARD_HEIGHT_BUF

#define BLOCK_MAX_SIZE 4

typedef struct pixel *pixelptr;
typedef struct pixel {
    int x;
    int y;
    int type;
    int color;
} pixel;

typedef struct block *blockptr;
typedef struct block {
    pixelptr board[BLOCK_MAX_SIZE][BLOCK_MAX_SIZE];
} block;

typedef struct {
    pixelptr board[BOARD_HEIGHT][BOARD_WIDTH];
    blockptr next_block;
    int score;
} game_data;

#endif