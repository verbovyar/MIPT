#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdint>

typedef double elem_t;

struct myStack {
#ifdef STACK_DEBUG
  elem_t left_cnry_ = 0;
#endif

  elem_t* array_ = NULL;
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

//------
void push(myStack* stack, int value);
void pop(myStack* stack);
void deleteStack(myStack* stack);
void clearStack(myStack* stack);
double top(myStack* stack);
uint64_t stackHash(myStack* stack);
myStack* newStack(size_t start_size);

//------

#ifdef STACK_DEBUG
#define ASSERTOK(stack) if (stackOk(stack) != NO_ERROR) { stackDump(stack);assert(!"OK");	}
#else 
#define ASSERTOK(stack) ;
#endif 

//------

enum STACK_ERROR{
  SIZE_ERROR,
  POINTER_ERROR,
  POP_ERROR,
  STACK_POINTER_ERROR,
  CONSTRUCT_ERROR,
  STRUCT_ERROR,
  ARRAY_ERROR,
  HASH_ERROR,
  NO_ERROR
};
void stackDump(myStack* stack);
STACK_ERROR stackOk(myStack* stack);