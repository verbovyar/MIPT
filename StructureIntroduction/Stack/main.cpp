#include "mystack.h"

int main()
{
    myStack* stack = NewStack(15);

    Push(stack, 10);
    Pop(stack);
    Pop(stack);

    return 0;
}