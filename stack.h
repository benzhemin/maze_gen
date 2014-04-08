#ifndef STACK_H
#define STACK_H

#include "predef.h"

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10

typedef struct {
	void *base;
	void *top;
	unsigned typesize;
	unsigned stacksize;
}SqStack;


bool init_stack(SqStack *s, unsigned typesize);
void destory_stack(SqStack *s);

bool stack_empty(SqStack *s);

bool get_top(SqStack *s, void *elem);

bool pop(SqStack *s, void *elem);

void push(SqStack *s, void *elem);

#endif