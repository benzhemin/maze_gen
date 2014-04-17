#include <curses.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "predef.h"
#include "display.h"
#include "maze_gen.h"

static WINDOW *maze_gen_wptr;
static WINDOW *maze_map_wptr;

#define COLS_START_X 10
#define COLS_OFFSET 15

void init_screen(int maze_rows, int maze_cols){
	int start_gen_x = 0;
	int start_map_x = 0;
	int screen_center_x = 0;
    int trans_row = maze_rows*2+1;
    int trans_col = maze_cols*2+1;
	maze_cols = maze_cols * 2;
    trans_col = trans_col * 2;

	initscr();

	screen_center_x = COLS/2;
	start_gen_x = screen_center_x/2 - maze_cols/2;
	//start_map_x = screen_center_x+screen_center_x/2 - maze_cols;
	start_map_x = COLS_START_X + maze_cols + COLS_OFFSET;

	maze_gen_wptr = newwin(maze_rows, maze_cols, 5, COLS_START_X);
	assert(maze_gen_wptr != NULL);
	wrefresh(maze_gen_wptr);

	maze_map_wptr = newwin(trans_row, trans_col, 5, start_map_x);
	assert(maze_map_wptr != NULL);
	wrefresh(maze_map_wptr);

	if (!has_colors()){
		endwin();
		exit(1);
	}
	if (start_color() != OK){
		endwin();
		exit(0);
	}
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_WHITE, COLOR_WHITE);
	wattron(maze_map_wptr, COLOR_PAIR(1));
}


void destory_screen(){
	delwin(maze_gen_wptr);
	delwin(maze_map_wptr);

	endwin();
}

void print_maze_gen(Node *maze, int maze_rows, int maze_cols){
	int row, col;
	for(row=0; row<maze_rows; row++){
		for(col=0; col<maze_cols; col++){
			mvwprintw(maze_gen_wptr, row, col*2, "%d ", (maze + row*maze_cols + col)->visited);
		}
	}
	wrefresh(maze_gen_wptr);
}
 
 
void print_maze_trans(Maze *maze){
    int row, col;
    for(row=0; row<maze->maze_rows; row++){
        for(col=0; col<maze->maze_cols; col++){
            Node *np = maze->nodes + row*maze->maze_cols + col;
            printf("%d ", np->visited);
            fflush(stdout);
        }
        printf("\n");
    }
    printf("\n");
}

void print_maze_map(Maze *maze){
    int TRANS_ROW = maze->maze_rows*2+1;
    int TRANS_COL = maze->maze_cols*2+1;
    
    int *maze_map = malloc(sizeof(int) *  TRANS_ROW*TRANS_COL);
    memset(maze_map, 0, sizeof(int) * TRANS_ROW*TRANS_COL);
    
    
    int row, col;
    /*
    for (row=0; row<maze->maze_rows; row++) {
        for (col=0; col<maze->maze_cols; col++) {
            Node *np = maze->nodes+row*maze->maze_cols+col;
            printf("%d ", np->visited);
        }
        printf("\n");
    }
    printf("\n");
    */
     
    for (row=0; row<maze->maze_rows; row++) {
        for (col=0; col<maze->maze_cols; col++) {
            Node *np = maze->nodes+row*maze->maze_cols+col;
            
            if (np->visited == TRUE) {
                int trans_per_row = maze->maze_cols*2+1;
                int trans_row = row * 2 + 1;
                int trans_col = col * 2 + 1;
                
                *(maze_map + trans_row*trans_per_row + trans_col) = TRUE;
                
                Direct di;
                for (di=Di_East; di<Di_LEN; di++) {
                    int new_trans_row = trans_row;
                    int new_trans_col = trans_col;
                    if (np->di[di] == TRUE) {
                        switch (di) {
                            case Di_East:
                                new_trans_col++;
                                break;
                            case Di_West:
                                new_trans_col--;
                                break;
                            case Di_Sorth:
                                new_trans_row++;
                                break;
                            case Di_North:
                                new_trans_row--;
                                break;
                            default:
                                break;
                        }
                        
                        if ((new_trans_col>0&&new_trans_col<trans_per_row) && (new_trans_row>0&&new_trans_row<maze->maze_rows*2+1)) {
                            *(maze_map + new_trans_row*trans_per_row+new_trans_col) = TRUE;
                        }
                    }
                }
            }
        }
    }
    
    /*
    for (row=0; row<TRANS_ROW; row++) {
        for (col=0; col<TRANS_COL; col++) {
            printf("%d ", *(maze_map + row*TRANS_COL + col));
        }
        printf("\n");
    }
    printf("\n");
    */
	
    for (row=0; row<maze->maze_rows; row++) {
        for (col=0; col<maze->maze_cols; col++) {
            mvwprintw(maze_gen_wptr, row, col*2, "%d ", (maze->nodes+row*maze->maze_cols+col)->visited);
        }
    }
    wrefresh(maze_gen_wptr);
    
    
	for(row=0; row<TRANS_ROW; row++){
		for(col=0; col<TRANS_COL; col++){
            if (*(maze_map + row*TRANS_COL + col) != 0) {
                mvwprintw(maze_map_wptr, row, col*2, "  "/*, *(maze_map + row*TRANS_COL + col)-1 /*'*'*/);
            }
		}
	}
	wrefresh(maze_map_wptr);
	
	wattron(maze_map_wptr, COLOR_PAIR(2));
	mvwprintw(maze_map_wptr, 1, 2, "  ");
	mvwprintw(maze_map_wptr, TRANS_ROW-2, TRANS_COL*2-4, "  ");
	wattron(maze_map_wptr, COLOR_PAIR(1));
	wrefresh(maze_map_wptr);

	free(maze_map);
}

void print_maze(Maze *maze){
	//print_maze_gen(maze, MAZE_ROW, MAZE_COL);
	print_maze_map(maze);
}