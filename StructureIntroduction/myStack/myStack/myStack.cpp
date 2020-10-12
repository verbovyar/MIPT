#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "mystack.h"

int main() {
  myStack* stck = newStack(5);
  
  for (int i = 0; i < 5; ++i) {
    push(stck, 10);
  }

  stck->array_[2] = 7;
  top(stck);

  return 0;
}


