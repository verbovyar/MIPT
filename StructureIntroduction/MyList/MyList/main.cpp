#include "list.h"

const int START_SIZE = 10;

int main()
{
    List* list = NewList(START_SIZE);

    pushHead(list, 9);
    pushHead(list, 10);
    pushHead(list, 11);
    pushHead(list, 12); // 9 10 11 12 13 14 17
    pushHead(list, 13); // 9 10 11 -100 12 13 14 17
    pushHead(list, 14); // 9 10 11 -200 -100 12 13 14 17
    pushHead(list, 17); // 9 10 11 -200 +100 -100 12 13 14 17
                        // 9 10 11 -200 +200 +100 -100 12 13 14 17
                        // 9 10 11 -200 + 200 +100 -100 12 13 14 17 5000 6000
                        // 10 11 -200 + 200 +100 -100 12 13 14 17 5000 6000
                        // 10 11 -200 + 200 +100 -100 12 13 14 17 5000 
                        // 10 -200 + 200 +100 -100 12 13 14 17 5000

    int32_t real_idx = findIdx(list, 4); // :12 
    pushBeforeFindIdx(list, -100, real_idx);
    pushBeforeFindIdx(list, -200, real_idx);
    pushAfterIdx(list, +100, real_idx);
    pushAfterIdx(list, +200, real_idx);
    
    pushTail(list, 5000);
    pushTail(list, 6000);

    popHead(list);
    popTail(list);

    real_idx = findIdx(list, 2);
    popInIdx(list, real_idx);

    real_idx = findIdx(list, 1);
    printf("Numb:[%d]", getValue(list, real_idx));

    listDumpReal(list);
    //listDumpLogical(list);

    freeList(list);

    return 0;
}

