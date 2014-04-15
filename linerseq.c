#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "predef.h"
#include "linerseq.h"

bool init_linerseq(SqList *sq, unsigned typesize){
	sq->typesize = typesize;
	sq->elem = malloc(sq->typesize * LIST_INIT_SIZE);
	if(sq->elem == NULL){
		perror("memory alloc failure");
	}	
	sq->length = 0;
	sq->listsize = LIST_INIT_SIZE;
	return TRUE;	
}

void destory_linerseq(SqList *sq){
	free(sq->elem);
}

void check_needs_expand(SqList *sq){
	if(sq->length >= sq->listsize){
		sq->elem = realloc(sq->elem, (sq->listsize+LIST_INCREMENT)*sq->typesize);
		if(sq->elem == NULL){
			perror("memory alloc failure");
		}
		sq->listsize += LIST_INCREMENT;
	}
}

void insert_inerseq_tail(SqList *sq, void *pe){
	unsigned step = sq->typesize;
	check_needs_expand(sq);

	char *p = (char *)sq->elem + step*sq->length;
	memcpy(p, pe, sq->typesize);
	++sq->length;
}

void insert_linerseq(SqList *sq, void *pe){
	return insert_linerseq_index(sq, sq->length+1, pe);
}

// 1<=index<=sq->length+1
void insert_linerseq_index(SqList *sq, int index, void *pe){
	assert(index>0);
	check_needs_expand(sq);

	unsigned step = sq->typesize;
	char *p, *q;

	q = (char *)sq->elem+(index-1)*step;
	p=(char *)sq->elem+(sq->length-1)*step;

	for(; p>=q && sq->length; p=p-step){
		memcpy(p+step, p, sq->typesize);
	}

	memcpy(q, pe, sq->typesize);
	++sq->length;
}

void elem_at_index(SqList *sq, int index, void *pe){
	assert(index <= sq->length);

	unsigned step = sq->typesize;
	char *elem = sq->elem;

	memcpy(pe, elem+step*index, step);
}

void print_linerseq(SqList *sq, void visit(void *elem)){
	char *p;
	for(p=sq->elem; p<(char *)sq->elem+sq->length*sq->typesize; p=p+sq->typesize){
		visit(p);
	}
	printf("\n");
}