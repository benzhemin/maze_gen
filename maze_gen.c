#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "maze_gen.h"
#include "stack.h"
#include "linerseq.h"
#include "display.h"

#define MAZE_ROW 2
#define MAZE_COL 3

void init_maze(Node (*maze)[MAZE_COL]){
	int row,col;
	for(row=0; row<MAZE_ROW; row++){
		for(col=0; col<MAZE_COL; col++){
			Node *node = &maze[row][col];
			node->point.col_pos = col;
			node->point.row_pos = row;
			for (int i=0; i<Di_LEN; i++) {
                node->di[i] = FALSE;
            }
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
	if((p.col_pos>=0 && p.col_pos< MAZE_COL) && (p.row_pos>=0 && p.row_pos< MAZE_ROW)){
		return TRUE;
	}
	return FALSE;
}

bool has_unvisited_neighbour(Node (*maze)[MAZE_COL], Node *p, SqList *L){
	bool has_neighbour = FALSE;
	Direct di;
	for(di=Di_East; di<=Di_West; di++){
		Point cur;
        cur.row_pos = p->point.row_pos;
        cur.col_pos = p->point.col_pos;

		switch(di){
			case Di_East:
				cur.col_pos++;
				break;
			case Di_North:
				cur.row_pos--;
				break;
			case Di_Sorth:
				cur.row_pos++;
				break;
			case Di_West:
				cur.col_pos--;
				break;
			default:
				perror("direction error!");
				break;
		}

		if(is_inbounds(cur) && maze[cur.row_pos][cur.col_pos].visited==FALSE){
			Node n;
			n.point = cur;
			n.di[di] = TRUE;
            n.visited = FALSE;
			
			insert_linerseq(L, &n);

			has_neighbour = TRUE;
		}
	}
	return has_neighbour;
}

void random_neighbour(SqList *L, Node *pn){
	unsigned len = L->length;
	srand((unsigned)clock());
	int randnum = rand() % len;

	elem_at_index(L, randnum, pn);
}

void remove_wall(Node (*maze)[MAZE_COL], Node *cur, Node *next){
	int x_offset = next->point.col_pos - cur->point.col_pos;
	int y_offset = next->point.row_pos - cur->point.row_pos;

	Direct di = -1;
	if (x_offset == 1){
		di = Di_East;
	}else if (x_offset == -1){
		di = Di_West;
	}

	if(y_offset == 1){
		di = Di_Sorth;
	}else if (y_offset == -1){
		di = Di_North;
	}
    assert(di >= 0);
    
	Point curp = cur->point;
	maze[curp.row_pos][curp.col_pos].di[di] = TRUE;
}


void print_maze_origin(Node (*maze)[MAZE_COL]){
    int row, col;
    for(row=0; row<MAZE_ROW; row++){
        for(col=0; col<MAZE_COL; col++){
            printf("%d ", maze[row][col].visited);
        }
        printf("\n");
    }
    printf("\n");
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
			
			cur->visited = TRUE;
            
            Node nei_node;
            random_neighbour(&neighbours, &nei_node);
			remove_wall(maze, cur, &nei_node);

			push(&s, cur);
            
            print_maze_origin(maze);
            print_maze_trans(maze[0], MAZE_ROW, MAZE_COL);
			//print_maze(maze[0], MAZE_ROW, MAZE_COL);

			cur = &maze[nei_node.point.row_pos][nei_node.point.col_pos];
            //cur->visited = TRUE;
			//sleep(1);

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

	//init_screen(MAZE_ROW, MAZE_COL);

	init_maze(maze);
	
	depth_first_gen(maze);

	//destory_screen();

	return 0;
}

