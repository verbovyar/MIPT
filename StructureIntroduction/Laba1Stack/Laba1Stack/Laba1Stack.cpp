#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct dynamicArray {
    int* array = NULL;
    int size = 0; 
    int capacity = 0;
    int increaseFactor = 2;
};

void construct(struct dynamicArray* stack, int startSize)
{
    assert(stack != NULL);

    stack->array = (int*)calloc(startSize, sizeof(int));
    stack->capacity = startSize;
}

void push(struct dynamicArray* stack, int value)
{
    assert(stack != NULL);

    if (stack->size == stack->capacity)
    {
        int* temp = (int*)realloc(stack->array, stack->capacity * stack->increaseFactor);
        if (temp == NULL)
        {
            return;
        }
        stack->array = temp; 
        stack->capacity *= stack->increaseFactor;

        free(temp);
    }

    stack->array[stack->size] = value;
    ++stack->size;
}

void pop(struct dynamicArray* stack)
{
    assert(stack != NULL);

    int index = stack->size - 1;
    stack->array[index] = 0;
    --stack->size;
}

int top(struct dynamicArray* stack)
{
    assert(stack != NULL);

    int index = stack->size - 1;
    return stack->array[index];
}

void destroy(struct dynamicArray* stack)
{
    free(stack->array);
    stack->size = 0;
    stack->capacity = 0;
}
 
int main()
{
    dynamicArray stack;
    construct(&stack, 10);

    for (int i = 0; i < 12; ++i)
    {
        push(&stack, 5);
    }

    int x = top(&stack);
    pop(&stack);

    printf("%d %d", stack.array[11], x);

    return 0;
}
