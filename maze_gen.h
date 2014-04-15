#ifndef _MAZE_GEN_H
#define _MAZE_GEN_H

#include "predef.h"

typedef struct{
	int x;
	int y;
}Point;

typedef enum{
	Di_None = 0,
	Di_East,
	Di_Sorth,
	Di_North,
	Di_West,
}Direct;

typedef struct {
	Point point;
	Direct di;
	bool visited;
}Node;


#endif