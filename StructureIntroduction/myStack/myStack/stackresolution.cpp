#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <chrono>
#include "mystack.h"

struct myStack {
    double* array_ = NULL;
    int size_ = 0;
    int capacity_ = 0;
    double increase_factor_ = 2.5;
};

void construct(myStack* array_stack, size_t start_size)
{
    assert(array_stack != NULL);

    array_stack->array_ = (double*)calloc(start_size, sizeof(double));
    array_stack->capacity_ = start_size;
    isOk(array_stack);
    for (int i = 0; i < start_size; ++i)
    {
        array_stack->array_[i] = NAN;
    }

    isOk(array_stack);
}

myStack* newStack(size_t start_size)
{
    myStack* array_stack = (myStack*)calloc(1, sizeof(myStack));
    construct(array_stack, start_size);

    return array_stack;
}

void stackRealloc(myStack* array_stack)
{
    isOk(array_stack);

    if (array_stack->size_ == array_stack->capacity_)
    {
        double* temp = (double*)realloc(array_stack->array_, array_stack->capacity_ * array_stack->increase_factor_ * sizeof(double));
        if (temp == NULL)
        {
            return;
        }

        for (int i = array_stack->size_; i < array_stack->capacity_; ++i)
        {
            array_stack->array_[i] = NAN;
        }
        array_stack->array_ = temp;
        array_stack->capacity_ *= array_stack->increase_factor_;
    }

    isOk(array_stack);
}

void shrinkToFit(myStack* array_stack)
{
    if (array_stack->size_ * 2 == array_stack->capacity_)
    {
        double* temp = (double*)realloc(array_stack->array_, array_stack->capacity_ / 2 * sizeof(double));
        if (temp == NULL)
        {
            return;
        }

        array_stack->array_ = temp;
        array_stack->capacity_ /= 2;
    }
}

void push(myStack* array_stack, int value)
{
    isOk(array_stack);

    assert(array_stack != NULL);

    stackRealloc(array_stack);

    array_stack->array_[array_stack->size_] = value;
    ++array_stack->size_;

    isOk(array_stack);
}

void pop(myStack* array_stack)
{
    isOk(array_stack);

    assert(array_stack != NULL);

    int index = array_stack->size_ - 1;
    array_stack->array_[index] = NAN;
    --array_stack->size_;

    isOk(array_stack);
}

double top(myStack* array_stack)
{
    assert(array_stack != NULL);

    isOk(array_stack);

    int index = array_stack->size_ - 1;

    isOk(array_stack);

    return array_stack->array_[index];
}

void destroy(myStack* array_stack)
{
    isOk(array_stack);

    assert(array_stack != NULL);

    free(array_stack->array_);
    array_stack->size_ = 0;
    array_stack->capacity_ = 0;

    isOk(array_stack);
}

void clearStack(myStack* array_stack)
{
    isOk(array_stack);

    assert(array_stack != NULL);

    for (int i = 0; i < array_stack->size_; ++i)
    {
        array_stack->array_[i] = NAN;
    }
    array_stack->array_ = NULL;
    array_stack->capacity_ = 0;
    array_stack->size_ = 0;

    isOk(array_stack);
}

void isOk(myStack* array_stack)
{
    if (array_stack == NULL)
    {
        stackDump(CONSTRUCT_ERROR, array_stack);
        assert(!"OK");
    }

    if (array_stack->array_ == NULL)
    {
        stackDump(POINTER_ERROR, array_stack);
        assert(!"OK");
    }

    if (array_stack->size_ == -1)
    {
        stackDump(POP_ERROR, array_stack);
        assert(!"OK");
    }

    if (array_stack->capacity_ < array_stack->size_)
    {
        stackDump(SIZE_ERROR, array_stack);
        assert(!"OK");
    }

    if (array_stack->capacity_ < 0)
    {
        stackDump(STACK_POINTER_ERROR, array_stack);
        assert(!"OK");
    }
}

void stackDump(STACK_ERROR ERROR, myStack* array_stack)
{
    switch (ERROR)
    {
    case(SIZE_ERROR):
    {
        printf("Out of array!\n");
        printf("Capacity:%d\n", array_stack->capacity_);
        printf("Size:%d\n", array_stack->size_);

        break;
    }
    case(STACK_POINTER_ERROR):
    {
        printf("Stack array error!\n");
        printf("Capacity:%d\n", array_stack->capacity_);
        printf("Size:%d\n", array_stack->size_);

        break;
    }
    case(POINTER_ERROR):
    {
        printf("Memory array stack error!\n");
        printf("Pointer is NULL:%d\n", array_stack->array_);
        printf("Capacity:%d\n", array_stack->capacity_);

        break;
    }
    case(POP_ERROR):
    {
        printf("Stack is empty, you cant pop element!\n");
        printf("Stack size:%d\n", array_stack->size_);

        break;
    }
    case(CONSTRUCT_ERROR):
    {
        printf("Stack construct error!\n");
        printf("Pointer is NULL:%d\n", array_stack);

        break;
    }
    }
}