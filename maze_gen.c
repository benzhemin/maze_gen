#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze_gen.h"
#include "stack.h"
#include "linerseq.h"
#include "display.h"

#define MAZE_ROW 2
#define MAZE_COL 2

void init_maze(Node (*maze)[MAZE_COL]){
	int row,col;
	for(row=0; row<MAZE_ROW; row++){
		for(col=0; col<MAZE_COL; col++){
			Node *node = &maze[row][col];
			node->point.x = col;
			node->point.y = row;
			node->di = Di_None;
			node->visited = FALSE;
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

		if(is_inbounds(cur) && maze[cur.y][cur.x].visited==FALSE){
			Node n;
			n.point = cur;
			n.di = di;
			
			insert_linerseq(L, &n);

			has_neighbour = TRUE;
		}
	}
	return has_neighbour;
}

void random_neighbour(SqList *L, Node *pn){
	unsigned len = L->length;
	srand((unsigned) time(NULL));
	int randnum = rand() % len;

	elem_at_index(L, randnum, pn);
}

void remove_wall(Node (*maze)[MAZE_COL], Node *cur, Node *next){
	int x_offset = next->point.x - cur->point.x;
	int y_offset = next->point.y - next->point.y;

	Direct di = Di_None;
	if (x_offset == 1){
		di = Di_East;
	}else if (x_offset == -1){
		di = Di_West;
	}

	if(y_offset == 1){
		di = Di_North;
	}else if (y_offset == -1){
		di = Di_Sorth;
	}

	Point curp = cur->point;
	maze[curp.y][curp.x].di = di;
}

void depth_first_gen(Node (*maze)[MAZE_COL]){
	Node *cur;
	SqStack s;
	init_stack(&s, sizeof(Node *));

	cur = maze[0];
	while(has_unvisited_node(maze)){
		SqList neighbours;
		init_linerseq(&neighbours, sizeof(Node));

		if(has_unvisited_neighbour(maze, cur, &neighbours)){	
			Node nei_node;
			random_neighbour(&neighbours, &nei_node);
			remove_wall(maze, cur, &nei_node);

			cur->visited = TRUE;
			push(&s, cur);

			cur = &maze[nei_node.point.y][nei_node.point.x];
            cur->visited = TRUE;
            
			print_maze((Node **)maze, MAZE_ROW, MAZE_COL);

			sleep(2);

		}else if(!stack_empty(&s)){
			pop(&s, cur);

		}else{
			//pick a node as cur, and mark as visited
			break;
		}

		destory_linerseq(&neighbours);
	}

	destory_stack(&s);
}

int main(void){
	Node maze[MAZE_ROW][MAZE_COL];

	init_screen(MAZE_ROW, MAZE_COL);

	init_maze(maze);
	
	depth_first_gen(maze);

	destory_screen();

	return 0;
}

