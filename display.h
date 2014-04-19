#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "maze_gen.h"

void init_screen(int maze_rows, int maze_cols);
void destory_screen();

void transform_maze(Maze *maze, TransMaze *trans_maze);

void write_maze_map(Maze *maze);

void print_maze_gen(Maze *maze);
void print_trans_maze(TransMaze *maze_map);

void print_maze(Maze *maze);

#endif