#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "mystack.h"

int main() {
  myStack* stck = newStack(3);
  for (int i = 0; i < 5; ++i) {
    push(stck, 10);
  }
  push(stck, 7);
  printf("%lf\n", top(stck));
  pop(stck);
  printf("%lf", top(stck));

  return 0;
}


