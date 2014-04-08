#ifndef _LINER_SEQ_H
#define _LINER_SEQ_H

#include "predef.h"

#define LIST_INIT_SIZE 10000
#define LIST_INCREMENT 1000

typedef int (*CMP)(const void *pa, const void *pb);

typedef struct _SqList{
	void *elem;
	unsigned typesize;
	unsigned length;
	unsigned listsize;
} SqList;

//initialize liner
bool init_linerseq(SqList *sq, unsigned typesize);

//destory liner
void destory_linerseq(SqList *sq);

//insert at tail
void insert_linerseq(SqList *sq, void *pe);

//optimized insert at tail
void insert_inerseq_tail(SqList *sq, void *pe);

//insert element to linerSeq
//index starts from 1
void insert_linerseq_index(SqList *sq, int index, void *pe);

void elem_at_index(SqList *sq, int index, void *pe);
//print all elements
void print_linerseq(SqList *sq, void visit(void *elem));

#endif
