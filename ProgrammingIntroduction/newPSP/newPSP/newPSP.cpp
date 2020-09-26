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
    else if (dynamic_stack->size_ == 0)
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

    if (dynamic_stack.size_ == 0)
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
