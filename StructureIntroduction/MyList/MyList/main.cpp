#include "list.h"

const int START_SIZE = 30;

int main()
{
    List* list = newList(START_SIZE);

    pushHead(list, 9);                   // :9
    pushHead(list, 10);                  // :10 9
    pushHead(list, 11);                  // :11 10 9
    pushHead(list, 12);                  // :12 11 10 9
    pushHead(list, 13);                  // :13 12 11 10 9
    pushHead(list, 14);                  // :14 13 12 11 10 9
    pushHead(list, 17);                  // :17 14 13 12 11 10 9

    listDumpLogical(list);

    int32_t real_idx = findIdx(list, 4); // :(12) 
    pushBeforeIdx(list, -100, real_idx); // :17 14 13 -100 12 11 10 9
    pushBeforeIdx(list, -200, real_idx); // :17 14 13 -100 -200 12 11 10 9
    pushAfterIdx(list, +200, real_idx);  // :17 14 13 -100 -200 12 +200 11 10 9
    pushAfterIdx(list, +100, real_idx);  // :17 14 13 -100 -200 12 +200 +100 11 10 9

    listDumpLogical(list);
    
    pushTail(list, 5000);    // :17 14 13 -100 -200 12 +200 +100 11 10 9 5000
    pushTail(list, 6000);    // :17 14 13 -100 -200 12 +200 +100 11 10 9 5000 6000

    listDumpLogical(list);

    popHead(list);           // :14 13 -100 -200 12 +200 +100 11 10 9 5000 6000
    popTail(list);           // :14 13 -100 -200 12 +200 +100 11 10 9 5000

    listDumpLogical(list);

    real_idx = findIdx(list, 1);         // :(14)
    printf("Numb:[%d]", getValue(list, real_idx)); // :(14)

    //listDumpReal(list);
    listDumpLogical(list);

    freeList(list);

    return 0;
}

