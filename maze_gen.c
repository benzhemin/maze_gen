#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "maze_gen.h"
#include "stack.h"
#include "linerseq.h"
#include "display.h"

void init_maze(Maze *maze, int maze_rows, int maze_cols){
    maze->nodes = (Node *)malloc(sizeof(Node) * maze_rows *maze_cols);
    maze->maze_rows = maze_rows;
    maze->maze_cols = maze_cols;
    
	int row,col;
	for(row=0; row<maze_rows; row++){
		for(col=0; col<maze_cols; col++){
			Node *node = maze->nodes + row*maze_cols + col;
			node->point.col_pos = col;
			node->point.row_pos = row;
			for (int i=0; i<Di_LEN; i++) {
                node->di[i] = FALSE;
            }
			node->visited = FALSE;
		}
	}
}

bool has_unvisited_node(Maze *maze){
	int row, col;
	for(row=0; row<maze->maze_rows; row++){
		for(col=0; col<maze->maze_cols; col++){
            Node *pn = maze->nodes + row*maze->maze_cols + col;
            if (pn->visited == FALSE) {
                return TRUE;
            }
        }
	}

	return FALSE;
}

bool is_inbounds(Maze *maze, Point p){
	if((p.col_pos>=0 && p.col_pos< maze->maze_cols) && (p.row_pos>=0 && p.row_pos< maze->maze_rows)){
		return TRUE;
	}
	return FALSE;
}

bool has_unvisited_neighbour(Maze *maze, Node *p, SqList *L){
	bool has_neighbour = FALSE;
	Direct di;
	for(di=Di_East; di<=Di_West; di++){
		Point cur = p->point;
        
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

        Node *pn = maze->nodes + cur.row_pos*maze->maze_cols + cur.col_pos;
		if(is_inbounds(maze, cur) && pn->visited==FALSE){
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

void remove_wall(Maze *maze, Node *cur, Node *next){
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
	Node *np = maze->nodes + curp.row_pos*maze->maze_cols + curp.col_pos;
    np->di[di] = TRUE;
}

void depth_first_gen(Maze *maze){
	SqStack s;
	init_stack(&s, sizeof(Node *));

	Node *cur = maze->nodes;
	while(has_unvisited_node(maze)){
		SqList neighbours;
		init_linerseq(&neighbours, sizeof(Node));

		if(has_unvisited_neighbour(maze, cur, &neighbours)){	
			
			cur->visited = TRUE;
            
            Node nb_node;
            random_neighbour(&neighbours, &nb_node);
			remove_wall(maze, cur, &nb_node);

			push(&s, cur);
            
            cur = maze->nodes+nb_node.point.row_pos*maze->maze_cols+nb_node.point.col_pos;
            cur->visited = TRUE;
			usleep(20*1000);
            
            print_maze(maze);
            
		}else if(!stack_empty(&s)){
			pop(&s, cur);

		}else{
			//pick a node as cur, and mark as visited
			break;
		}

		destory_linerseq(&neighbours);
	}
    
    print_maze(maze);

	destory_stack(&s);
}

void destory_maze(Maze *maze){
    free(maze->nodes);
}

int main(int argc, char *argv[]){
    int maze_rows, maze_cols;
    if (argc != 3) {
        perror("maze size must be specified!");
        exit(EXIT_FAILURE);
    }
    
    maze_rows = atoi(argv[1]);
    maze_cols = atoi(argv[2]);
    
    Maze maze;
    init_screen(maze_rows, maze_cols);
    
    init_maze(&maze, maze_rows, maze_cols);
	
	depth_first_gen(&maze);

    scanf("%d", &maze_rows);
    
    destory_maze(&maze);

	destory_screen();

	return 0;
}

