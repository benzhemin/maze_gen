#include <curses.h>
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
	maze_cols = maze_cols * 2;

	initscr();

	screen_center_x = COLS/2;
	start_gen_x = screen_center_x/2 - maze_cols/2;
	//start_map_x = screen_center_x+screen_center_x/2 - maze_cols;
	start_map_x = COLS_START_X + maze_cols + COLS_OFFSET;

	maze_gen_wptr = newwin(maze_rows, maze_cols, 5, COLS_START_X);
	assert(maze_gen_wptr != NULL);
	box(maze_gen_wptr, '|', '-');
	wrefresh(maze_gen_wptr);

	maze_map_wptr = newwin(maze_rows*2, maze_cols*2, 5, start_map_x);
	assert(maze_map_wptr != NULL);
	box(maze_map_wptr, '|', '-');
	wrefresh(maze_map_wptr);
}

void destory_screen(){
	delwin(maze_gen_wptr);
	delwin(maze_map_wptr);

	touchwin(stdscr);
	refresh();
	endwin();
}

/*
void print_maze(Node (*maze)[MAZE_COL]){
	int row, col;
	for(row=0; row<MAZE_ROW; row++){
		for(col=0; col<MAZE_COL; col++){
			printf("%d ", maze[row][col].visited);
		}
		printf("\n");
	}
    printf("\n");
}
*/

void print_maze_gen(Node **maze, int MAZE_ROW, int MAZE_COL){
	int row, col;
	for(row=0; row<MAZE_ROW; row++){
		for(col=0; col<MAZE_COL; col++){
			mvwprintw(maze_gen_wptr, row, col*2, "%d ", (*((Node *)maze + row*MAZE_COL + col)).visited);
		}
	}
	wrefresh(maze_gen_wptr);
}

void print_maze_map(Node **maze, int MAZE_ROW, int MAZE_COL){
	int row, col;
	int maze_rows = MAZE_ROW*2;
	int maze_cols = MAZE_COL*2;
	int maze_size = maze_rows * maze_cols;
	int *maze_map = (int *)malloc(sizeof(int) * maze_size);
	memset(maze_map, 0, sizeof(int) * maze_rows * maze_cols);

	for(row=0; row<MAZE_ROW; row++){
		for(col=0; col<MAZE_COL; col++){
			Node *node = (Node *)maze + row*MAZE_COL + col;
			if(node->visited == TRUE){
				*(maze_map + row*maze_cols + col*2) = 1;
			}

			switch(node->di){
				case Di_None:
					break;
				case Di_East:
					*(maze_map + row*maze_cols + col*2+1) = 1;
					break;
				case Di_Sorth:
					if (((row+1)*maze_cols + col*2) < maze_size){
						*(maze_map + (row+1)*maze_cols + col*2) = 1;	
					}
					break;
				case Di_North:
					if((row-1) >= 0){
						*(maze_map + (row-1)*maze_cols + col*2) = 1;		
					}
					break;
				case Di_West:
					*(maze_map + row*maze_cols + col*2-1) = 1;
					break;
				default:
					assert(FALSE);
			}
		}
	}

	for(row=0; row<maze_rows; row++){
		for(col=0; col<maze_cols; col++){
			mvwprintw(maze_map_wptr, row, col*2, "%d ", *(maze_map+row*MAZE_COL+col));
		}
	}
	wrefresh(maze_map_wptr);
}

void print_maze(Node **maze, int MAZE_ROW, int MAZE_COL){
	print_maze_gen(maze, MAZE_ROW, MAZE_COL);
	print_maze_map(maze, MAZE_ROW, MAZE_COL);
}