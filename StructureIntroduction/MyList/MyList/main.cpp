#include "list.h"

const int START_SIZE = 30;

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

    int32_t real_idx = findIdx(list, 5); // :12 
    pushBeforeIdx(list, -100, real_idx);
    pushBeforeIdx(list, -200, real_idx);
    pushAfterIdx(list, +200, real_idx);
    pushAfterIdx(list, +100, real_idx);
    
    pushTail(list, 5000);
    pushTail(list, 6000);

    popHead(list);
    popTail(list);

    real_idx = findIdx(list, 1);
    printf("Numb:[%d]", getValue(list, real_idx));

    //listDumpReal(list);
    listDumpLogical(list);

    freeList(list);

    return 0;
}

