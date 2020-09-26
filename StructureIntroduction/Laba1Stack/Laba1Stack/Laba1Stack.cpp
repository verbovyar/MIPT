#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const int MAX_SIZE = 100002;

struct dynamicArray {
    char* array_ = NULL;
    int size_ = 0;
    int capacity_ = 0;
    int increase_factor_ = 2;
};

void construct(dynamicArray* array_stack, int start_size)
{
    assert(array_stack != NULL);

    array_stack->array_ = (char*)calloc(start_size, sizeof(int));
    array_stack->capacity_ = start_size;
}

void push(dynamicArray* array_stack, int value)
{
    assert(array_stack != NULL);

    if (array_stack->size_ == array_stack->capacity_)
    {
        char* temp = (char*)realloc(array_stack->array_, array_stack->capacity_ * array_stack->increase_factor_);
        if (temp == NULL)
        {
            return;
        }
        array_stack->array_ = temp;
        array_stack->capacity_ *= array_stack->increase_factor_;

        free(temp);
    }

    array_stack->array_[array_stack->size_] = value;
    ++array_stack->size_;
}

void pop(dynamicArray* array_stack)
{
    assert(array_stack != NULL);

    int index = array_stack->size_ - 1;
    array_stack->array_[index] = 0;
    --array_stack->size_;
}

char top(dynamicArray* array_stack)
{
    assert(array_stack != NULL);

    int index = array_stack->size_ - 1;
    return array_stack->array_[index];
}

void destroy(dynamicArray* array_stack)
{
    assert(array_stack != NULL);

    free(array_stack->array_);
    array_stack->size_ = 0;
    array_stack->capacity_ = 0;
}
 
int main()
{
    dynamicArray array_stack;
    construct(&array_stack, 10);

    for (int i = 0; i < 12; ++i)
    {
        push(&array_stack, 5);
    }

    int x = top(&array_stack);
    pop(&array_stack);

    printf("%d %d", array_stack.array_[11], x);

    return 0;
}
