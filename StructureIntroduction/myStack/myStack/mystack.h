#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <chrono>

//------

struct myStack;

void shrinkToFit(myStack* array_stack);
void push(myStack* array_stack, int value);
void pop(myStack* array_stack);
void destroy(myStack* array_stack);
void clearStack(myStack* array_stack);
double top(myStack* array_stack);
myStack* newStack(size_t start_size);

//------

enum STACK_ERROR {
    SIZE_ERROR = 101,
    POINTER_ERROR = 202,
    POP_ERROR = 303,
    STACK_POINTER_ERROR = 404,
    CONSTRUCT_ERROR = 505
};
void stackDump(STACK_ERROR ERROR, myStack* array_stack);
void isOk(myStack* array_stack);