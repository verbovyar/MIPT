#include "list.h"

const int START_SIZE = 10;

int main()
{
    List* list = NewList(START_SIZE);

    pushHead(list, 9);
    pushHead(list, 10);
    pushHead(list, 11);
    pushHead(list, 12);
    pushHead(list, 13);
    pushHead(list, 14);
    pushHead(list, 17);

    pushInIdx(list, 34, 3);

    freeList(list);

    return 0;
}
