#define _CRT_SECURE_NO_WARNINGS

#include "mystack.h"

typedef double elem_t;

const elem_t LEFT_CNRY = 0x212121;
const elem_t RIGHT_CNRY = 0x121212;
const elem_t POISON_VALUE = 1e+308;

struct myStack {

  #ifdef STACK_DEBUG
  int hash_summ = 0;
  #endif
  
  #ifdef STACK_DEBAG
  elem_t left_cnry_ = 0;
  #endif

  elem_t* array_ = NULL;
  int size_ = 0;
  int capacity_ = 0;
  int increase_factor_ = 0; 

  #ifdef STACK_DEBAG
  elem_t right_cnry_ = 0;
  #endif
};

void construct(myStack* stack, size_t start_size) {
  assert(stack != NULL);

  #ifdef STACK_DEBUG
  stack->left_cnry_ = LEFT_CNRY;
  stack->right_cnry_ = RIGHT_CNRY;

  elem_t* temp = (elem_t*)calloc(1, (2 + start_size) * sizeof(elem_t));
  if (temp == NULL) {
    return;
  }
  *(elem_t*)stack->array_ = LEFT_CNRY;
  *(elem_t*)(stack->array_ + sizeof(elem_t) + start_size * sizeof(elem_t)) = RIGHT_CNRY;
  
  stack->hash_summ = stackHash(stack);
    
  #else

  elem_t* temp = (elem_t*)calloc(1, start_size * sizeof(elem_t));
  if (temp == NULL) {
    return;
  }

  #endif

  stack->array_ = temp;
  stack->capacity_ = start_size;
  stack->increase_factor_ = 2;

  for (int i = 1; i <= start_size; ++i) {
    stack->array_[i] = POISON_VALUE;
  }
}

myStack* newStack(size_t start_size) {
  myStack* stack = (myStack*)calloc(1, sizeof(myStack));
  construct(stack, start_size);

  return stack;
}

void stackNewSize(myStack* stack) {
  assert(stack != NULL);
    
  ASSERTOK(stack)

  if (stack->size_ == stack->capacity_) {

    #ifdef STACK_DEBUG
    elem_t* temp = (elem_t*)calloc(1, (stack->capacity_ + 2) * stack->increase_factor_ * sizeof(elem_t));  
    if (temp == NULL) {
      return;
    }

    free(stack->array_);
    *(elem_t*)stack->array_ = LEFT_CNRY;
    *(elem_t*)(stack->array_ + sizeof(elem_t) + (stack->capacity_ * stack->increase_factor_) * sizeof(elem_t)) = RIGHT_CNRY;

    stack->hash_summ = stackHash(stack);
    #else

    elem_t* temp = (elem_t*)calloc(1, stack->capacity_ * stack->increase_factor_ * sizeof(elem_t));
    if (temp == NULL) {
      return;
    }
    #endif

    stack->array_ = temp;
    stack->capacity_ *= stack->increase_factor_; 
    for (int i = stack->size_; i < stack->capacity_; ++i) {
      stack->array_[i] = POISON_VALUE;
    }
  }
}

void shrinkToFit(myStack* stack) {
  assert(stack != NULL);

  #ifdef STACK_DEBUG
  if (stack->size_ * stack->increase_factor_ + 1 == stack->capacity_) {
    elem_t* temp = (elem_t*)calloc(1, (2 + stack->capacity_) / stack->increase_factor_ * sizeof(elem_t));
    if (temp == NULL) {
      return;
    }

    *(elem_t*)stack->array_ = LEFT_CNRY;
    *(elem_t*)(stack->array_ + sizeof(elem_t) + (stack->capacity_ / stack->increase_factor_) * sizeof(elem_t)) = RIGHT_CNRY;

    stack->hash_summ = stackHash(stack);
    #else
  elem_t* temp = (elem_t*)calloc(1, stack->capacity_ / stack->increase_factor_ * sizeof(elem_t));
  if (temp == NULL) {
    return;
  }
  #endif
    stack->array_ = temp;
    stack->capacity_ /= stack->increase_factor_;
}

void push(myStack * stack, int value) {
  assert(stack != NULL);

  ASSERTOK(stack)

  stackNewSize(stack);

  ++stack->size_;
  stack->array_[stack->size_] = value;

  #ifdef DEBUG
  stack->hash_summ = stackHash(stack);
  #endif
}

void pop(myStack * stack) {
  assert(stack != NULL);

  ASSERTOK(stack)

  stack->array_[stack->size_] = POISON_VALUE;
  --stack->size_;

  #ifdef DEBUG
  stack->hash_summ = stackHash(stack);
  #endif
}

elem_t top(myStack* stack) {
  assert(stack != NULL);

  ASSERTOK(stack)

  elem_t value = stack->array_[stack->size_];

  ASSERTOK(stack)

  #ifdef DEBUG
  stack->hash_summ = stackHash(stack);
  #endif

  return value;
}

void deleteStack(myStack* stack) {
  assert(stack != NULL);

  ASSERTOK(stack)

  free(stack->array_);
  stack->size_ = 0;
  stack->capacity_ = 0;
  free(stack);

  #ifdef DEBUG
  stack->hash_summ = stackHash(stack);
  #endif
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

  #ifdef DEBUG
  stack->hash_summ = stackHash(stack);
  #endif
}

#ifdef STACK_DEBUG
  uint64_t stackHash(myStack * stack) {
  int hash_summ = 0;
  const char* temp = (const char*)stack;

  uint64_t size_of_hash = sizeof(myStack);
  for (int i = 0; i < size_of_hash; ++i) {
    int bite = temp[i]; 
    hash_summ += (bite * i);
  }

  return hash_summ;
}

 bool hashOk(myStack* stack) {
    uint62_t temp_hash = stack->hash_summ;  
    uint64_t new_hash = stackHash(stack);

    if (temp_hash != new_hash) {
      return HASH_ERROR;
    }

    return NO_ERROR;
 }

#endif

STACK_ERROR stackOk(myStack* stack) {

  if (stack == NULL) {
    return CONSTRUCT_ERROR;
  }

  #ifdef STACK_DEBUG 
  if (stack->left_cnry_ != LEFT_CNRY || stack->right_cnry_ != RIGHT_CNRY) {
    return STRUCT_ERROR;
  }
  #endif

  #ifdef STACK_DEBUG 
  if (HASH_ERROR) {
    return HASH_ERROR;
  }
  #endif

  #ifdef STACK_DEBUG
  if (stack->array_[0] != LEFT_CNRY || stack->array_[stack->capacity + 1] != RIGHT_CNRY) {
    return ARRAY_ERROR;
  }
#endif

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

    #ifdef STACK_DEBUG
    case (HASH_ERROR): {
      fprintf(log_file, "Hash_error, you cant to change buffer!");
    }
    #endif
    
    #ifdef STACK_DEBUG
    case (STRUCT_ERROR): {
      fprintf(log_file, "You cant to change struct(stack)!\n");
      fprintf(log_file, "Right CNRY:%u \n",stack->right_cnry_);
      fprintf(log_file, "Real CNRY:%u", RIGHT_CNRY);
      fprintf(log_file, "Left CNRY:%u \n", stack->left_cnry_);
      fprintf(log_file, "Real CNRY:%u", LEFT_CNRY);

      break;
    }
    #endif
    
    #ifdef STACK_DEBUG
    case (ARRAY_ERROR): {
      fprintf(log_file, "You cant to change array(stack)!\n");
      fprintf(log_file, "Right CNRY:%u \n", stack->array_[0]);
      fprintf(log_file, "Real CNRY:%u", RIGHT_CNRY);
      fprintf(log_file, "Left CNRY:%u \n", stack->array_[stack->capacity_ + 1]);
      fprintf(log_file, "Real CNRY:%u", LEFT_CNRY);
    }
    #endif
  }

  fclose(log_file);
}