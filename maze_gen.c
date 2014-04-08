#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#include "linerseq.h"

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

#define MAZE_ROW 8
#define MAZE_COL 8

void init_maze(Node (*maze)[MAZE_COL]){
	int row,col;
	for(row=0; row<MAZE_ROW; row++){
		for(col=0; col<MAZE_COL; col++){
			Node node = maze[row][col];
			node.point.x = row;
			node.point.y = col;
			node.di = Di_None;
			node.visited = FALSE;
		}
	}
}

bool has_unvisited_node(Node (*maze)[MAZE_COL]){
	int row, col;
	for(row=0; row<MAZE_ROW; row++){
		for(col=0; col<MAZE_COL; col++){
			if(maze[row][col].visited == FALSE){
				return TRUE;
			}
		}
	}

	return FALSE;
}

bool is_inbounds(Point p){
	if((p.y>=0 && p.y< MAZE_COL) && (p.x>=0 && p.x< MAZE_ROW)){
		return TRUE;
	}
	return FALSE;
}

bool has_unvisited_neighbour(Node (*maze)[MAZE_COL], Node *p, SqList *L){
	bool has_neighbour = FALSE;
	Direct di;
	for(di=Di_East; di<=Di_West; di++){
		Point cur = {p->point.x, p->point.y};

		switch(di){
			case Di_East:
				cur.x++;
				break;
			case Di_North:
				cur.y--;
				break;
			case Di_Sorth:
				cur.y++;
				break;
			case Di_West:
				cur.x--;
				break;
			default:
				perror("direction error!");
				break;
		}

		if(is_inbounds(cur) && maze[cur.x][cur.y].visited==FALSE){
			Node n;
			n.point = cur;
			n.di = di;
			
			insert_linerseq(L, &n);

			has_neighbour = TRUE;
		}
	}
	return has_neighbour;
}

void depth_first_gen(Node (*maze)[MAZE_COL]){
	Node *cur;
	SqStack s;
	init_stack(&s, sizeof(Node *));

	cur = maze[0];
	cur->visited = TRUE;
	while(has_unvisited_node(maze)){
		SqList neighbours;
		init_linerseq(&neighbours, sizeof(Node));

		if(has_unvisited_neighbour(maze, cur, &neighbours)){
			unsigned len = neighbours.length;
			unsigned rand_num = srand(time(NULL))%len;
			Node nodep;
			elem_at_index(&neighbours, &nodep);

			push(&s, cur);

		}else if(!empty_stack(&s)){


		}else{
			//pick a node as cur, and mark as visited

		}

		destory_linerseq(&neighbours);
	}

	destory_stack(&s);
}

int main(void){
	Node maze[MAZE_ROW][MAZE_COL];

	init_maze(maze);

	depth_first_gen(maze);

	return 0;
}

