#include "stack.h"

#ifdef STACK_DEBUG

//-------------------
//Function that counts stack hash.
//hash = 1) sum of bites * bit's idx
//     + 2) sum of elements * elements idx 
//-------------------
uint64_t StackHash(myStack* stack) 
{
    assert(stack != NULL);

    stack->     hash_summ = 0;
    int         hash_summ = 0;
    const char* temp      = (const char*)stack;

    uint64_t size_of_hash = sizeof(myStack);

    for (int i = 0; i < size_of_hash; ++i) 
    {
        int bite   = temp[i];
        hash_summ += (bite * i);
    }

    for (int i = 0; i < stack->capacity_ + 1; ++i) 
    {
        hash_summ += i * (uint64_t)stack->array_[i];
    }

    return hash_summ;
}

//-------------------
//Function is checking hash.
//If hash isnt correct ->
//function returns HASH_ERROR
//-------------------
STACK_ERROR hashOk(myStack* stack) 
{
    assert(stack != NULL);

    uint64_t temp_hash = stack->hash_summ;
    uint64_t new_hash  = StackHash(stack);

    if (temp_hash != new_hash) { return HASH_ERROR; }
    stack->hash_summ = new_hash;

    return NO_ERROR;
}

#endif

//-------------------
//Function creates new stack
//and fills fields:
//1)left and right canaries
//2)capacity
//3)allocates memory for array
//4)set increase factor
//5)count hash
//6)fills array elements of poison value
//-------------------
void Construct(myStack* stack, size_t start_size) 
{
    assert(stack != NULL);

#ifdef STACK_DEBUG

    stack->left_cnry_  = LEFT_CNRY;
    stack->right_cnry_ = RIGHT_CNRY;

    elem_t* temp = (elem_t*)calloc(1, (2 + start_size) * sizeof(elem_t));
    if (temp == NULL) { return; }

    stack->array_ = temp;

     *(elem_t*)stack->array_                       = LEFT_CNRY;
    *((elem_t*)stack->array_ + 1 + start_size * 1) = RIGHT_CNRY;

#else

    elem_t* temp = (elem_t*)calloc(1, start_size * sizeof(elem_t));
    if (temp == NULL) { return; }

#endif

//-----
    stack->array_           = temp;
    stack->capacity_        = start_size;
    stack->increase_factor_ = INCREASE_FACTOR;

    for (int i = 1; i <= start_size; ++i) 
    {
        stack->array_[i] = POISON_VALUE;
    }
//-----

#ifdef STACK_DEBUG

    stack->hash_summ = StackHash(stack);

#endif

}

//-------------------
//Function allocates memory
//for stack struct and calls Construct
//-------------------
myStack* NewStack(size_t start_size)
{
    myStack* stack = (myStack*)calloc(1, sizeof(myStack));

    Construct(stack, start_size);

    return stack;
}

//-------------------
//Function resize stack
//array and rewrite stack info:
//canaries, capacity, size, hash
//-------------------
void StackNewSize(myStack* stack) 
{
    assert  (stack != NULL);
    ASSERTOK(stack)

    if (stack->size_ == stack->capacity_) 
    {
        #ifdef STACK_DEBUG

            elem_t* temp = (elem_t*)calloc(1, (stack->capacity_ * 
                                               stack->increase_factor_ + 2) * sizeof(elem_t));
            if (temp == NULL) { return; }

            free(stack->array_);

            stack->array_ = temp;

            *((elem_t*)stack->array_ + (stack->capacity_ * 
                       stack->increase_factor_ + 1)) = RIGHT_CNRY;
             *(elem_t*)stack->array_                 = LEFT_CNRY;

        #else

            elem_t* temp = (elem_t*)calloc(1, stack->capacity_ * 
                                              stack->increase_factor_);
            if (temp == NULL) { return; }

        #endif

            stack->capacity_ *= stack->increase_factor_;

            for (int i = stack->size_; i < stack->capacity_; ++i)
            {
                stack->array_[i] = POISON_VALUE;
            }

        #ifdef STACK_DEBUG

            stack->hash_summ = StackHash(stack);

        #endif
    }
}

//-------------------
//Function resize stack array
//if much less capacity
//and rewrite stack info:
//canaries, capacity, size, hash
//-------------------
void ShrinkToFitToFit(myStack* stack)
{
  assert(stack != NULL);

#ifdef STACK_DEBUG

  if (stack->size_ * stack->increase_factor_ + 1 == stack->capacity_) 
  {
      elem_t* temp = (elem_t*)calloc(1, (2 + stack->capacity_) / 
                                             stack->increase_factor_ * sizeof(elem_t));
      if (temp == NULL) { return; }
      stack->array_ = temp;

      stack->capacity_ /= stack->increase_factor_;

      *(elem_t*)(stack->array_ + sizeof(elem_t) + 
                (stack->capacity_ / 
                 stack->increase_factor_) * 
                sizeof(elem_t)) = RIGHT_CNRY;
      *(elem_t*)stack->array_   = LEFT_CNRY;

      stack->hash_summ = StackHash(stack);
  }

#else

  elem_t* temp = (elem_t*)calloc(1, stack->capacity_ / 
                                    stack->increase_factor_ * sizeof(elem_t));
  if (temp == NULL) { return; }

#endif

}

//-------------------
//Function inserts an element 
//to the top of stack and call
//StackNewSize - set new stack
//array size
//-------------------
void Push(myStack * stack, int value) 
{
    assert(stack != NULL);

    ASSERTOK(stack)

    StackNewSize(stack);

    ++stack->size_;

    stack->array_[stack->size_] = value;

#ifdef STACK_DEBUG

    stack->hash_summ = StackHash(stack);

#endif

    ASSERTOK(stack)
}

//-------------------
//Function pulls an element 
//to the top of stack and
// delete it, after that
//set new stack array size
//-------------------
void Pop(myStack* stack) 
{
    assert(stack != NULL);

    ASSERTOK(stack)

    stack->array_[stack->size_] = POISON_VALUE;
    --stack->size_;

#ifdef STACK_DEBUG

    stack->hash_summ = StackHash(stack);

#endif

    ASSERTOK(stack)
}

//-------------------
//Function pulls an element 
//to the top of stack and
//set new stack array size
//-------------------
elem_t Top(myStack* stack) 
{
    assert(stack != NULL);

    ASSERTOK(stack)

    elem_t value = stack->array_[stack->size_];

    ASSERTOK(stack)

    return value;
}

//-------------------
int StackSize   (myStack* stack)
{
    return stack->size_;
}
//-------------------

//-------------------
//Function delete stack
//set size and capacity = 0
//and free stack pointer
//and stack array
//-------------------
void DeleteStack(myStack* stack) 
{
    assert(stack != NULL);

    ASSERTOK(stack)

    stack->array_    = NULL;
    stack->size_     = 0;
    stack->capacity_ = 0;

    free(stack);

#ifdef STACK_DEBUG

    stack->hash_summ = StackHash(stack);

#endif

}

//-------------------
//Function clear stack info
//set size and capacity = 0
//set stack cotens of poison value
//set stack pointer
//and stack array = NULL
//-------------------
void ClearStack(myStack* stack) 
{
    assert(stack != NULL);

    ASSERTOK(stack)

    for (int i = 0; i < stack->size_; ++i) 
    {
       stack->array_[i] = POISON_VALUE;
    }

    stack->array_    = NULL;
    stack->capacity_ = 0;
    stack->size_     = 0;
    stack            = NULL;

#ifdef STACK_DEBUG

    stack->hash_summ = StackHash(stack);

#endif
}

//-------------------
//Function function checks the stack
//and if stack is incorrect
//return stack error value to 
//stack dump
//-------------------
STACK_ERROR stackOk(myStack* stack) 
{
    if (stack == NULL) 
    {
        return CONSTRUCT_ERROR;
    }

#ifdef STACK_DEBUG

    if (stack->left_cnry_ != LEFT_CNRY || stack->right_cnry_ != RIGHT_CNRY) 
    {
        return STRUCT_ERROR;
    }

#endif

#ifdef STACK_DEBUG

    if (hashOk(stack) != NO_ERROR) 
    {
        return HASH_ERROR;
    }

#endif

#ifdef STACK_DEBUG

    if (stack->array_[0] != LEFT_CNRY || stack->array_[stack->capacity_ + 1] != RIGHT_CNRY) 
    {
        return ARRAY_ERROR;
    }

#endif

    if (stack->array_ == NULL) 
    {
        return POINTER_ERROR;
    }

    if (stack->size_ == -1) 
    {
        return POP_ERROR;
    }

    if (stack->capacity_ < stack->size_) 
    {
        return SIZE_ERROR;
    }

    if (stack->capacity_ < 0) 
    {
        return STACK_POINTER_ERROR;
    }

    return NO_ERROR;
}

//-------------------
//Function accept stack error
//and outputs all information 
//about stack to the log file
//-------------------
void stackDump(myStack* stack) 
{
    FILE*       log_file = fopen("logfile.txt", "w");
    STACK_ERROR ERROR    = stackOk(stack);

    switch (ERROR) 
    {
        case (SIZE_ERROR): 
        {
            fprintf(log_file, "Out of array!\n\n");
            fprintf(log_file, "Capacity:%d\n", stack->capacity_);
            fprintf(log_file, "Size:%d\n\n", stack->size_);

            for (int i = 1; i <= stack->size_; ++i) 
            {
                fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->array_[i]);
            }

            break;
        }
        case (STACK_POINTER_ERROR): 
        {
            fprintf(log_file, "Stack array error!\n\n");
            fprintf(log_file, "Capacity:%d\n", stack->capacity_);
            fprintf(log_file, "Size:%d\n\n", stack->size_);
            for (int i = 1; i <= stack->size_; ++i)

            {
                fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->array_[i]);
            }

            break;
        }
        case (POINTER_ERROR): 
        {
            fprintf(log_file, "Memory array stack error!\n\n");
            fprintf(log_file, "Pointer is NULL:%p\n\n", stack->array_);
            fprintf(log_file, "Capacity:%d\n", stack->capacity_);
            fprintf(log_file, "Size:%d\n\n", stack->size_);

            for (int i = 1; i <= stack->size_; ++i) 
            {
                fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->array_[i]);
            }

            break;
        }
        case (POP_ERROR): 
        {
            fprintf(log_file, "Stack is empty, you cant Pop element!\n\n");
            fprintf(log_file, "Capacity:%d\n", stack->capacity_);
            fprintf(log_file, "Size:%d\n\n", stack->size_);

            for (int i = 1; i <= stack->size_; ++i) 
            {
                fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->array_[i]);
            }

            break;
        }
        case (CONSTRUCT_ERROR): 
        {
            fprintf(log_file, "Stack construct error!\n\n");
            fprintf(log_file, "Pointer is NULL:%p\n\n", stack);
            fprintf(log_file, "Capacity:%d\n", stack->capacity_);
            fprintf(log_file, "Size:%d\n\n", stack->size_);

            for (int i = 1; i <= stack->size_; ++i) 
            {
                fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->array_[i]);
            }

            break;
        }

#ifdef STACK_DEBUG
        case (HASH_ERROR): 
        {
            fprintf(log_file, "Hash_error, you cant to change buffer!\n\n");
            fprintf(log_file, "Capacity:%d\n", stack->capacity_);
            fprintf(log_file, "Size:%d\n\n", stack->size_);

            for (int i = 1; i <= stack->size_; ++i) 
            {
                fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->array_[i]);
            }

            break;
    }

#endif

#ifdef STACK_DEBUG

        case (STRUCT_ERROR): 
        {
            fprintf(log_file, "You cant to change struct(stack)!\n\n");
            fprintf(log_file, "Right CNRY:%lf \n", stack->right_cnry_);
            fprintf(log_file, "Real CNRY:%lf\n", RIGHT_CNRY);
            fprintf(log_file, "Left CNRY:%lf \n", stack->left_cnry_);
            fprintf(log_file, "Real CNRY:%lf\n\n", LEFT_CNRY);
            fprintf(log_file, "Capacity:%d\n", stack->capacity_);
            fprintf(log_file, "Size:%d\n\n", stack->size_);

            for (int i = 1; i <= stack->size_; ++i) 
            {
                fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->array_[i]);
            }

            break;
        }

#endif

#ifdef STACK_DEBUG

        case (ARRAY_ERROR): 
        {
            fprintf(log_file, "You cant to change array(stack)!\n\n");
            fprintf(log_file, "Left  CNRY:%lf \n", stack->array_[0]);
            fprintf(log_file, "Real  CNRY:%lf\n\n", LEFT_CNRY);
            fprintf(log_file, "Right CNRY:%lf \n", stack->array_[stack->capacity_ + 1]);
            fprintf(log_file, "Real  CNRY:%lf \n\n", RIGHT_CNRY);
            fprintf(log_file, "Capacity:%d\n", stack->capacity_);
            fprintf(log_file, "Size:%d\n\n", stack->size_);

            for (int i = 1; i <= stack->size_; ++i) 
            {
                fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->array_[i]);
            }

            break;
        }

#endif

    }

    fclose(log_file);
}
