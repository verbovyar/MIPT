#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct dynamicArray {
    char* array = NULL;
    int size = 0;
    int capacity = 0;
    int increaseFactor = 2;
};

void construct(dynamicArray* array_stack, int start_size)
{
    assert(array_stack != NULL);

    array_stack->array = (char*)calloc(start_size, sizeof(int));
    array_stack->capacity = start_size;
}

void push(dynamicArray* array_stack, int value)
{
    assert(array_stack != NULL);

    if (array_stack->size == array_stack->capacity)
    {
        char* temp = (char*)realloc(array_stack->array, array_stack->capacity * array_stack->increaseFactor);
        if (temp == NULL)
        {
            return;
        }
        array_stack->array = temp;
        array_stack->capacity *= array_stack->increaseFactor;

        free(temp);
    }

    array_stack->array[array_stack->size] = value;
    ++array_stack->size;
}

void pop(dynamicArray* array_stack)
{
    assert(array_stack != NULL);

    int index = array_stack->size - 1;
    array_stack->array[index] = 0;
    --array_stack->size;
}

int top(dynamicArray* array_stack)
{
    assert(array_stack != NULL);

    int index = array_stack->size - 1;
    return array_stack->array[index];
}

void destroy(dynamicArray* array_stack)
{
    assert(array_stack != NULL);

    free(array_stack->array);
    array_stack->size = 0;
    array_stack->capacity = 0;
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

    printf("%d %d", array_stack.array[11], x);

    return 0;
}
