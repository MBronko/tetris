#ifndef COMMON_H
#define COMMON_H

#define MENU_WIDTH 20
#define MENU_HEIGHT 10

#define SPACE_BETWEEN_WINDOWS 1

#define BOARD_WIDTH 22
#define BOARD_HEIGHT_VISIBLE 22
#define BOARD_HEIGHT_BUF 3
#define BOARD_HEIGHT_TOTAL BOARD_HEIGHT_VISIBLE+BOARD_HEIGHT_BUF

#define SIDE_PANEL_WIDTH 15

#define BLOCK_WINDOW_HEIGHT 6
#define LEGEND_WINDOW_HEIGHT 11
#define SCORE_WINDOW_HEIGHT 3

#define MIN_WINDOW_WIDTH (BOARD_WIDTH+SIDE_PANEL_WIDTH+SPACE_BETWEEN_WINDOWS)

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

typedef struct menu_data *menuptr;
typedef struct menu_data{
    int win_x;
    int win_y;
    WINDOW *win;
    char *rand_header;
    int game_active;
    int highlight;
    int quit;
} menu_data;

typedef struct game_data *gameptr;
typedef struct game_data{
    int win_x;
    int win_y;
    WINDOW * win_board;
    WINDOW * win_next;
    WINDOW * win_legend;
    WINDOW * win_score;
    pixelptr board[BOARD_HEIGHT_TOTAL][BOARD_WIDTH];
    blockptr act_block;
    int next_block;
    int score;
} game_data;

#endif