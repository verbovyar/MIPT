#pragma once
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
	char* data = NULL;
	Node* left = NULL;
	Node* right = NULL;
	Node* parent = NULL;
};

typedef Node* elem_t;

struct Stack {
#ifdef STACK_DEBUG
	elem_t left_cnry_ = 0;
#endif

	elem_t* data_ = NULL;
	int size_ = 0;
	int capacity_ = 0;
	int increase_factor_ = 0;

#ifdef STACK_DEBUG
	elem_t right_cnry_ = 0;
#endif

#ifdef STACK_DEBUG
	uint64_t hash_summ = 0;
#endif
};

void push(Stack* stack, elem_t value);
void pop(Stack* stack);
void deleteStack(Stack* stack);
void clearStack(Stack* stack);
elem_t top(Stack* stack);
uint64_t stackHash(Stack* stack);
Stack* newStack(size_t start_size);


#ifdef STACK_DEBUG
#define ASSERTOK(stack) if (stackOk(stack) != NO_ERROR) { stackDump(stack);assert(!"OK");	}
#else 
#define ASSERTOK(stack) ;
#endif 

enum STACK_ERROR {
	SIZE_ERROR,
	POINTER_ERROR,
	POP_ERROR,
	STACK_POINTER_ERROR,
	CONSTRUCT_ERROR,
	STRUCT_ERROR,
	data_ERROR,
	HASH_ERROR,
	NO_ERROR
};
void stackDump(Stack* stack);
STACK_ERROR stackOk(Stack* stack);