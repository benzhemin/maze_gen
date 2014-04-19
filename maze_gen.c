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

void destory_maze(Maze *maze){
    free(maze->nodes);
}

void destory_trans_maze(TransMaze *trans_maze){
    free(trans_maze->maze_unit);
    free(trans_maze);
}

bool has_unvisited_node(Maze *maze, Node **punvisited){
	int row, col;
	for(row=0; row<maze->maze_rows; row++){
		for(col=0; col<maze->maze_cols; col++){
            Node *pn = maze->nodes + row*maze->maze_cols + col;
            if (pn->visited == FALSE) {
            	*punvisited = pn;
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

#define SLEEP_INTERNAL (50*1000)

void depth_first_gen(Maze *maze){
	SqStack s;
	init_stack(&s, sizeof(Node *));

	Node *cur = maze->nodes;
	Node *unvisited = NULL;
	while(has_unvisited_node(maze, &unvisited)){
		SqList neighbours;
		init_linerseq(&neighbours, sizeof(Node));

		if(has_unvisited_neighbour(maze, cur, &neighbours)){	
			
			cur->visited = TRUE;
            
            Node nb_node;
            random_neighbour(&neighbours, &nb_node);
			remove_wall(maze, cur, &nb_node);

			push(&s, cur);
            
            cur = maze->nodes+nb_node.point.row_pos*maze->maze_cols+nb_node.point.col_pos;

            print_maze(maze);

            usleep(SLEEP_INTERNAL);

		}else if(!stack_empty(&s)){
			cur->visited = TRUE;
			pop(&s, cur);

		}else{
			cur = unvisited;
			cur->visited = TRUE;
			print_maze(maze);
			usleep(SLEEP_INTERNAL);
		}

		destory_linerseq(&neighbours);
	}
    
    print_maze(maze);

	destory_stack(&s);
}

bool can_place_maze_row(TransMaze *trans_maze, int row, int col){
    int TRANS_COL = trans_maze->trans_cols;
    
    if (*(trans_maze->maze_unit+(row-1)*TRANS_COL+col)==1 && *(trans_maze->maze_unit+(row+1)*TRANS_COL+col)==1 &&
        *(trans_maze->maze_unit+row*TRANS_COL+col-1)==0 && *(trans_maze->maze_unit+row*TRANS_COL+col+1)==0) {
        return TRUE;
    }
    return FALSE;
}

bool can_place_maze_col(TransMaze *trans_maze, int row, int col){
    int TRANS_COL = trans_maze->trans_cols;
    
    if (*(trans_maze->maze_unit+row*TRANS_COL+col-1)==1 && *(trans_maze->maze_unit+row*TRANS_COL+col+1)==1 &&
        *(trans_maze->maze_unit+(row-1)*TRANS_COL+col)==0 && *(trans_maze->maze_unit+(row+1)*TRANS_COL+col)==0) {
        return TRUE;
    }
    return FALSE;
}

void print_tm(TransMaze *trans_maze){
    int row, col;
    for (row=0; row<trans_maze->trans_rows; row++) {
        for (col=0; col<trans_maze->trans_cols; col++) {
            printf("%d", *(trans_maze->maze_unit+row*trans_maze->trans_cols+col));
        }
        printf("\n");
    }
    printf("\n");
}

void write_trans_maze(TransMaze *trans_maze, char *file_name){
	FILE *fd = fopen(file_name, "w+");
	
	int row, col;
    for (row=0; row<trans_maze->trans_rows; row++) {
        for (col=0; col<trans_maze->trans_cols; col++) {
            fprintf(fd, "%d ", *(trans_maze->maze_unit+row*trans_maze->trans_cols+col));
        }
        fprintf(fd, "\n");
    }
    fprintf(fd, "\n");

    fclose(fd);
}

void randomize_maze(Maze *maze, TransMaze **ptrans_maze){
    TransMaze *trans_maze = malloc(sizeof(TransMaze));
    transform_maze(maze, trans_maze);
    *ptrans_maze = trans_maze;
    
    //print_tm(trans_maze);
    
    SqList points_list;
    int TRANS_ROWS = trans_maze->trans_rows;
    int TRANS_COLS = trans_maze->trans_cols;
    int row, col;
    for (row=1; row<TRANS_ROWS-1; row++) {
        init_linerseq(&points_list, sizeof(Point));
        for (col=0; col<TRANS_COLS; col++) {
            if (*(trans_maze->maze_unit+row*TRANS_COLS+col) == 0){
                if (can_place_maze_row(trans_maze, row, col)) {
                    Point p = {row, col};
                    insert_inerseq_tail(&points_list, &p);
                }
            }
        }
        
        if (points_list.length > 0) {
            Point p;
            srand((unsigned)clock());
            int randnum = rand() % points_list.length;
            
            elem_at_index(&points_list, randnum, &p);
            
            *(trans_maze->maze_unit+p.row_pos*TRANS_COLS+p.col_pos) = 1;
        }
        
        destory_linerseq(&points_list);
    }
    
    for(col=1; col<TRANS_COLS-1; col++){
        init_linerseq(&points_list, sizeof(Point));
        for (row=0; row<TRANS_ROWS; row++) {
            if (*(trans_maze->maze_unit+row*TRANS_COLS+col) == 0){
                if (can_place_maze_col(trans_maze, row, col)) {
                    Point p = {row, col};
                    insert_inerseq_tail(&points_list, &p);
                }
            }
        }
        
        if (points_list.length > 0) {
            Point p;
            srand((unsigned)clock());
            int randnum = rand() % points_list.length;
            
            elem_at_index(&points_list, randnum, &p);
            
            *(trans_maze->maze_unit+p.row_pos*TRANS_COLS+p.col_pos) = 1;
        }
        
        destory_linerseq(&points_list);
    }
    
    print_trans_maze(trans_maze);
}

int main(int argc, char *argv[]){
    int maze_rows, maze_cols;
    char *file_name;
    if (argc != 4) {
        perror("maze size and file name must be specified!");
        exit(EXIT_FAILURE);
    }
    
    maze_rows = atoi(argv[1]);
    maze_cols = atoi(argv[2]);
    file_name = argv[3];
    
    Maze maze;
    TransMaze *trans_maze;
    init_screen(maze_rows, maze_cols);
    
    init_maze(&maze, maze_rows, maze_cols);
	
	depth_first_gen(&maze);
    
    randomize_maze(&maze, &trans_maze);

    write_trans_maze(trans_maze, file_name);
    scanf("%d", &maze_rows);
    
    destory_trans_maze(trans_maze);
    destory_maze(&maze);

	destory_screen();

	return 0;
}

