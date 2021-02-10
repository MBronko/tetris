#ifndef MENU_H
#define MENU_H
#include "../tools/common.h"

extern char global_pause_header[];
extern char global_gameover_header[];

extern char *global_options[];
extern int global_n_options;

extern char *global_headers[];
extern int global_n_headers;

void menu();

#endif