#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdint>

//------

struct myStack;

void shrinkToFit(myStack* stack);
void push(myStack* stack, int value);
void pop(myStack* stack);
void destroy(myStack* stack);
void clearStack(myStack* stack);
double top(myStack* stack);
myStack* newStack(size_t start_size);

//------

#ifdef STACK_DEBUG
#define ASSERTOK(stack)             \
  if (stackOk(stack) != NO_ERROR) { \
	stackDump(stack);				\	
	assert(!"OK\nRead logfile");	\
  }
#else 
#define ASSERTOK(stack) ;
#endif 

//------

enum STACK_ERROR {
  SIZE_ERROR =          101,
  POINTER_ERROR =       202,
  POP_ERROR =           303,
  STACK_POINTER_ERROR = 404,
  CONSTRUCT_ERROR =     505,
  STRUCT_ERROR =        606,
  NO_ERROR =			0
};
void stackDump(myStack* stack);
STACK_ERROR stackOk(myStack* stack);