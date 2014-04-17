#ifndef _MAZE_GEN_H
#define _MAZE_GEN_H

#include "predef.h"

typedef struct{
	int row_pos;
	int col_pos;
}Point;

typedef enum{
	Di_East = 0,
	Di_Sorth,
	Di_North,
	Di_West,
    Di_LEN
}Direct;

typedef struct {
	Point point;
	int di[Di_LEN];
	int visited;
}Node;

typedef struct{
    Node *nodes;
    int maze_rows;
    int maze_cols;
} Maze;


#endif