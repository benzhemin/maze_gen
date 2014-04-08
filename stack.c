#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"

bool init_stack(SqStack *s, unsigned typesize){
	s->typesize = typesize;

	s->top = s->base = malloc(sizeof(typesize) * STACK_INIT_SIZE);
	assert(s->top!=NULL);
	s->stacksize = STACK_INIT_SIZE;

	return TRUE;
}

void destory_stack(SqStack *s){
	free(s->base);
}

bool stack_empty(SqStack *s){
	return s->top == s->base;
}

bool get_top(SqStack *s, void *elem){
	if(stack_empty(s)){
		return FALSE;
	}

	char *top_elem = (char *)s->top - s->typesize;
	memcpy(elem, top_elem, s->typesize);

	return TRUE;
}

bool pop(SqStack *s, void *elem){
	if(stack_empty(s)){
		return FALSE;
	}

	char *top_elem = (char *)s->top - s->typesize;
	memcpy(elem, top_elem, s->typesize);

	s->top = (char *)s->top - s->typesize;

	return TRUE;
}

void push(SqStack *s, void *elem){
	char *top = (char *)s->top;
	char *base = (char *)s->base;
	unsigned step = s->typesize;

	if((top-base)/step >= s->stacksize){
		s->base = realloc(s->base, (s->stacksize+STACK_INCREMENT)*step);
		assert(s->base != NULL);
		s->top = s->base + s->stacksize*step;
		s->stacksize += STACK_INCREMENT;
	}

	memcpy(s->top, elem, s->typesize);
	s->top = (char *)s->top + step;
}