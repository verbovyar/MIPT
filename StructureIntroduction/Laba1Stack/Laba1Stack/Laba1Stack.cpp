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

void construct(struct dynamicArray* arrayStack, int startSize)
{
    assert(arrayStack != NULL);

    arrayStack->array = (int*)calloc(startSize, sizeof(int));
    arrayStack->capacity = startSize;
}

void push(struct dynamicArray* arrayStack, int value)
{
    assert(arrayStack != NULL);

    if (arrayStack->size == arrayStack->capacity)
    {
        int* temp = (int*)realloc(arrayStack->array, arrayStack->capacity * arrayStack->increaseFactor);
        if (temp == NULL)
        {
            return;
        }
        arrayStack->array = temp; 
        arrayStack->capacity *= arrayStack->increaseFactor;

        free(temp);
    }

    arrayStack->array[arrayStack->size] = value;
    ++arrayStack->size;
}

void pop(struct dynamicArray* arrayStack)
{
    assert(arrayStack != NULL);

    int index = arrayStack->size - 1;
    arrayStack->array[index] = 0;
    --arrayStack->size;
}

int top(struct dynamicArray* arrayStack)
{
    assert(arrayStack != NULL);

    int index = arrayStack->size - 1;
    return arrayStack->array[index];
}

void destroy(struct dynamicArray* arrayStack)
{
    assert(arrayStack != 0);

    free(arrayStack->array);
    free(&arrayStack->size);
    free(&arrayStack->capacity);
}
 
int main()
{
    dynamicArray arrayStack;
    construct(&arrayStack, 10);

    for (int i = 0; i < 12; ++i)
    {
        push(&arrayStack, 5);
    }

    int x = top(&arrayStack);
    pop(&arrayStack);

    printf("%d %d", arrayStack.array[11], x);

    return 0;
}
