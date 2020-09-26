#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const int MAX_SIZE = 100002;

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

char* readFile()
{
    char* sequence = (char*)calloc(MAX_SIZE, sizeof(char));
    if (sequence == NULL)
    {
        exit(0);
    }
    fgets(sequence, MAX_SIZE, stdin);

    return sequence;
}

bool closeSymbol(char symbol, char temp_symbol, dynamicArray* dynamic_stack)
{
    switch (symbol)
    {
    case(']'):
    {
        return (temp_symbol == '[');

        break;
    }
    case('}'):
    {
        return (temp_symbol == '{');

        break;
    }
    case(')'):
    {
        return (temp_symbol == '(');

        break;
    }

    }
}

void symbolType(char symbol, dynamicArray* dynamic_stack)
{
    if (symbol == '[' || symbol == '{' || symbol == '(')
    {
        push(dynamic_stack, symbol);
    }
    else if (dynamic_stack->size == 0)
    {
        printf("NO");
        exit(0);
    }
    else
    {
        char temp_symbol = top(dynamic_stack);
        if (closeSymbol(symbol, temp_symbol, dynamic_stack))
        {
            pop(dynamic_stack);
        }
        else
        {
            printf("NO");
            exit(0);
        }
    }
}

int main()
{
    char* sequence = readFile();

    int index = 0;
    dynamicArray dynamic_stack;
    construct(&dynamic_stack, MAX_SIZE);
    while (sequence[index] != '\n')
    {
        symbolType(sequence[index], &dynamic_stack);
        ++index;
    }

    if (dynamic_stack.size == 0)
    {
        printf("YES\n");
    }
    else
    {
        printf("NO\n");
    }

    destroy(&dynamic_stack);

    return 0;
}
