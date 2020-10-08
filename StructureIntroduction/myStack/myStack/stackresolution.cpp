#define _CRT_SECURE_NO_WARNINGS

#include "mystack.h"

typedef double elem_t;

const uint32_t LEFT_CNRY = 0x212121;
const uint32_t RIGHT_CNRY = 0x121212;
const elem_t POISON_VALUE = 1e+308;

struct myStack {
  uint32_t left_cnry_ = 0;
  uint32_t right_cnry_ = 0;

  elem_t* array_ = NULL;
  int size_ = 0;
  int capacity_ = 0;
  int increase_factor_ = 0; 
};

void construct(myStack* stack, size_t start_size) {
  assert(stack != NULL);

  stack->left_cnry_ = LEFT_CNRY;
  stack->right_cnry_ = RIGHT_CNRY;
  elem_t* temp = (elem_t*)calloc(1, 2 * sizeof(uint32_t) + start_size * sizeof(elem_t));
  if (temp == NULL) {
    return;
  }
  stack->array_ = temp;
  *(uint32_t*)stack->array_ = LEFT_CNRY;
  *(uint32_t*)(stack->array_ + sizeof(uint32_t) + start_size * sizeof(elem_t)) = RIGHT_CNRY;
  stack->capacity_ = start_size;
  stack->increase_factor_ = 2;

  for (int i = 1; i <= start_size; ++i) {
    stack->array_[i] = POISON_VALUE;
  }

  ASSERTOK(stack);
}

myStack* newStack(size_t start_size) {
  myStack* stack = (myStack*)calloc(1, sizeof(myStack));
  construct(stack, start_size);

  return stack;
}

void stackRealloc(myStack* stack) {
  assert(stack != NULL);
    
  ASSERTOK(stack)

  if (stack->size_ == stack->capacity_) {
    elem_t* temp = (elem_t*)calloc(1, 2 * sizeof(uint32_t) + stack->capacity_ * stack->increase_factor_ * sizeof(elem_t));  
    if (temp == NULL) {
      return;
    }

    free(stack->array_);
    stack->array_ = temp;
    *(uint32_t*)stack->array_ = LEFT_CNRY;
    *(uint32_t*)(stack->array_ + sizeof(uint32_t) + (stack->capacity_ * stack->increase_factor_) * sizeof(elem_t)) = RIGHT_CNRY;
    stack->capacity_ *= stack->increase_factor_; 
    for (int i = stack->size_; i < stack->capacity_; ++i) {
      stack->array_[i] = POISON_VALUE;
    }
  }

  ASSERTOK(stack)
}

void shrinkToFit(myStack* stack) {
  assert(stack != NULL);

  if (stack->size_ * stack->increase_factor_ + 1 == stack->capacity_) {
    elem_t* temp = (elem_t*)calloc(1, 2 * sizeof(uint32_t) + stack->capacity_ / stack->increase_factor_ * sizeof(elem_t));
    if (temp == NULL) {
      return;
    }

    *(uint32_t*)stack->array_ = LEFT_CNRY;
    *(uint32_t*)(stack->array_ + sizeof(uint32_t) + (stack->capacity_ / stack->increase_factor_) * sizeof(elem_t)) = RIGHT_CNRY;
    stack->array_ = temp;
    stack->capacity_ /= stack->increase_factor_;
  }
}

void push(myStack* stack, int value) {
  assert(stack != NULL);

  ASSERTOK(stack)

  stackRealloc(stack);

  ++stack->size_;
  stack->array_[stack->size_] = value;

  ASSERTOK(stack)
}

void pop(myStack* stack) {
  assert(stack != NULL);

  ASSERTOK(stack)

  int index = stack->size_;
  stack->array_[index] = POISON_VALUE;
  --stack->size_;

  ASSERTOK(stack)
}

elem_t top(myStack* stack) {
  assert(stack != NULL);

  ASSERTOK(stack)

  int index = stack->size_;

  ASSERTOK(stack)

  return stack->array_[index];
}

void destroy(myStack* stack) {
  assert(stack != NULL);

  ASSERTOK(stack)

  free(stack->array_);
  stack->size_ = 0;
  stack->capacity_ = 0;
  free(stack);

  ASSERTOK(stack)
}

void clearStack(myStack* stack) {
  assert(stack != NULL);

  ASSERTOK(stack)

  for (int i = 0; i < stack->size_; ++i) {
    stack->array_[i] = POISON_VALUE;
  }
  stack->array_ = NULL;
  stack->capacity_ = 0;
  stack->size_ = 0;
  stack = NULL;

  ASSERTOK(stack)
}

STACK_ERROR stackOk(myStack* stack) {
  if (stack == NULL) {
    return CONSTRUCT_ERROR;
  }

  if (stack->left_cnry_ != LEFT_CNRY || stack->right_cnry_ != RIGHT_CNRY)
  {
    return STRUCT_ERROR;
  }

  if (stack->array_ == NULL) {
    return POINTER_ERROR;    
  }

  if (stack->size_ == -1) {
    return POP_ERROR;
  }

  if (stack->capacity_ < stack->size_) {
    return SIZE_ERROR;
  }

  if (stack->capacity_ < 0) {
    return STACK_POINTER_ERROR;
  }

  return NO_ERROR;
}

void stackDump(myStack* stack) {
  FILE* log_file = fopen("logfile.txt", "w");
  STACK_ERROR ERROR = stackOk(stack);
  switch (ERROR) {

    case (SIZE_ERROR): {
      fprintf(log_file,"Out of array!\n");
      fprintf(log_file, "Capacity:%d\n", stack->capacity_);
      fprintf(log_file, "Size:%d\n", stack->size_);

      break;
    }
    case (STACK_POINTER_ERROR): {
      fprintf(log_file, "Stack array error!\n");
      fprintf(log_file, "Capacity:%d\n", stack->capacity_);
      fprintf(log_file, "Size:%d\n", stack->size_);

      break;
    }
    case (POINTER_ERROR): {
      fprintf(log_file, "Memory array stack error!\n");
      fprintf(log_file, "Pointer is NULL:%p\n", stack->array_);
      fprintf(log_file, "Capacity:%d\n", stack->capacity_);

      break;
    }
    case (POP_ERROR): {
      fprintf(log_file, "Stack is empty, you cant pop element!\n");
      fprintf(log_file, "Stack size:%d\n", stack->size_);

      break;
    }
    case (CONSTRUCT_ERROR): {
      fprintf(log_file, "Stack construct error!\n");
      fprintf(log_file, "Pointer is NULL:%p\n", stack);

      break;
    }
    case (STRUCT_ERROR): {
      fprintf(log_file, "You cant to change struct(stack)!\n");
      fprintf(log_file, "Right CNRY:%u \n",stack->right_cnry_);
      fprintf(log_file, "Real CNRY:%u", RIGHT_CNRY);
      fprintf(log_file, "Left CNRY:%u \n", stack->left_cnry_);
      fprintf(log_file, "Real CNRY:%u", LEFT_CNRY);

      break;
    }
  }

  fclose(log_file);
}