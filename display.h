#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "maze_gen.h"

void init_screen(int maze_rows, int maze_cols);
void destory_screen();

void print_maze_trans(Maze *maze);

void print_maze(Maze *maze);

#endif