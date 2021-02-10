#ifndef COMMON_H
#define COMMON_H

#define MENU_WIDTH 20
#define MENU_HEIGHT 10

#define BOX_BORDER 2
#define SPACE_BETWEEN_WINDOWS 1

#define BOARD_GAME_WIDTH 10
#define BOARD_GAME_HEIGHT 20

#define BOARD_WIN_WIDTH (2*BOARD_GAME_WIDTH+BOX_BORDER)
#define BOARD_HEIGHT_BUF 5
#define BOARD_HEIGHT_TOTAL BOARD_GAME_HEIGHT+BOARD_HEIGHT_BUF

#define SIDE_PANEL_WIDTH (13+BOX_BORDER)

#define BLOCK_WINDOW_HEIGHT (4+BOX_BORDER)
#define LEGEND_WINDOW_HEIGHT (9+BOX_BORDER)
#define SCORE_WINDOW_HEIGHT (1+BOX_BORDER)

#define MIN_WINDOW_WIDTH (BOARD_WIN_WIDTH+SIDE_PANEL_WIDTH+SPACE_BETWEEN_WINDOWS)
#define MIN_WINDOW_HEIGHT (BOARD_GAME_HEIGHT+BOX_BORDER)

#define BLOCK_MAX_SIZE 4

#define KEY_ENTER_2 10

#define DIRECTION_RIGHT 1
#define DIRECTION_LEFT -1

#define GAME_STATE_ONGOING 0
#define GAME_STATE_QUIT 1
#define GAME_STATE_PAUSE 2
#define GAME_STATE_GAMEOVER 3

typedef struct block_str {
    int board[(BLOCK_MAX_SIZE)*(BLOCK_MAX_SIZE)];
    int n;
    int color;
    int x;
    int y;
    int rotation;
} block;

typedef struct menu_str *menuptr;
typedef struct menu_str{
    int win_x;
    int win_y;
    WINDOW *win;
    char *rand_header;
    bool game_active;
    int highlight;
    int menu_state;
} menu_data_str;

typedef struct game_str *gameptr;
typedef struct game_str{
    int win_x;
    int win_y;
    WINDOW * win_board;
    WINDOW * win_next;
    WINDOW * win_legend;
    WINDOW * win_score;
    int board[BOARD_HEIGHT_TOTAL][BOARD_GAME_WIDTH];
    block act_block;
    int next_block;
    int score;
} game_data_str;

#define min(x, y) ((x)<(y)?x:y)
#define max(x, y) ((x)>(y)?x:y)

#endif