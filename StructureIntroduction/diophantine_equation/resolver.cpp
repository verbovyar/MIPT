#include "stack.h"

const int START_SIZE = 500;

//-------------------
void Input       (int *a, int* b, int* c);
int  Resolver    (int a, int b, int* x, int* y);
void RestoreRoots(int* x, int* y, myStack* stack1);
void GetAnswer   (int c, int gcd, int x, int y);
//-------------------

void Output(myStack* stack)
{
    for (int i = 1; i <= stack->size_; ++i) 
    {
        printf("array[%d] = %d\n", i - 1, stack->array_[i]);
    }
    printf("\n");
}

int main()
{
    int x = 0;
    int y = 0;

    int a = 0;
    int b = 0;
    int c = 0;

    Input(&a, &b, &c);

    int gcd = Resolver(a, b, &x, &y);

    GetAnswer(c, gcd, x, y);

    return 0;
}

void Input(int *a, int* b, int* c)
{
    scanf("%d", a);

    scanf("%d", b);
    
    scanf("%d", c);
}

int Resolver(int a, int b, int* x, int* y)
{
    myStack* stack1 = NewStack(START_SIZE);

    Push(stack1, a);
    Push(stack1, b);

    Output(stack1);

    while (a != 0)
    {
        int temp = a;
        a = b % a;
        b = temp;

        if (a != 0)
        {
            Push(stack1, a);
            Push(stack1, b);

            Output(stack1);
        }
    }

    int gcd = b;

    printf("----------------------\n");

    RestoreRoots(x, y, stack1);

    DeleteStack(stack1);

    return gcd;
    
}

void RestoreRoots(int* x, int* y, myStack* stack1)
{
    myStack* stack2 = NewStack(START_SIZE);

    Push(stack2, 1);
    Push(stack2, 0);

    while (StackSize(stack1) != 0)
    {
        int x1 = Top(stack2);
        Pop(stack2);

        int y1 = Top(stack2);
        Pop(stack2);

        int b  = Top(stack1);
        Pop(stack1);

        int a  = Top(stack1);
        Pop(stack1);

        int x_temp = y1 - (b / a) * x1;
        int y_temp = x1;

        Push(stack2, y_temp);
        Push(stack2, x_temp);

        Output(stack2);
    }

    *x = Top(stack2);
    Pop(stack2);
    *y = Top(stack2);
    Pop(stack2);

    DeleteStack(stack2);
}

void GetAnswer(int c, int gcd, int x, int y)
{
    if (c % gcd == 0) 
    {
        x = x * c / gcd;
        y = y * c / gcd;

        printf("Answer: x = %d, y = %d\n", x, y);
    } 
    else 
    {
        printf("No solution");
    }
}