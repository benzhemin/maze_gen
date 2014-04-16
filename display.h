#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "maze_gen.h"

void init_screen(int maze_rows, int maze_cols);
void destory_screen();

void print_maze_trans(Node *maze, int MAZE_ROW, int MAZE_COL);

void print_maze(Node *maze, int MAZE_ROW, int MAZE_COL);

#endif