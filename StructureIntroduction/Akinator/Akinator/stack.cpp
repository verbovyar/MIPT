#include "stack.h"

const int LEFT_CNRY = 0x212121;
const int RIGHT_CNRY = 0x121212;
const int POISON_VALUE = 1e+308;

#ifdef STACK_DEBUG
uint64_t stackHash(Stack* stack)
{
    assert(stack != NULL);

    stack->hash_summ = 0;
    int hash_summ = 0;
    const char* temp = (const char*)stack;

    uint64_t size_of_hash = sizeof(Stack);
    for (int i = 0; i < size_of_hash; ++i)
    {
        int bite = temp[i];
        hash_summ += (bite * i);
    }

    for (int i = 0; i < stack->capacity_ + 1; ++i)
    {
        hash_summ += i * (uint64_t)stack->data_[i];
    }

    return hash_summ;
}

STACK_ERROR hashOk(Stack* stack)
{
    assert(stack != NULL);

    uint64_t temp_hash = stack->hash_summ;
    uint64_t new_hash = stackHash(stack);

    if (temp_hash != new_hash)
    {
        return HASH_ERROR;
    }
    stack->hash_summ = new_hash;

    return NO_ERROR;
}

#endif

void construct(Stack* stack, size_t start_size)
{
    assert(stack != NULL);

#ifdef STACK_DEBUG
    stack->left_cnry_ = LEFT_CNRY;
    stack->right_cnry_ = RIGHT_CNRY;

    elem_t* temp = (elem_t*)calloc(1, (2 + start_size) * sizeof(elem_t));
    if (temp == NULL)
    {
        return;
    }
    stack->data_ = temp;

    *(elem_t*)stack->data_ = LEFT_CNRY;
    *((elem_t*)stack->data_ + 1 + start_size * 1) = RIGHT_CNRY;

#else

    elem_t* temp = (elem_t*)calloc(1, start_size * sizeof(elem_t));
    if (temp == NULL)
    {
        return;
    }

#endif

    stack->data_ = temp;
    stack->capacity_ = start_size;
    stack->increase_factor_ = 2;

#ifdef STACK_DEBUG
    stack->hash_summ = stackHash(stack);
#endif;
}

Stack* newStack(size_t start_size)
{

    Stack* stack = (Stack*)calloc(1, sizeof(Stack));
    construct(stack, start_size);

    return stack;
}

void stackNewSize(Stack* stack)
{
    assert(stack != NULL);
    ASSERTOK(stack)

        if (stack->size_ == stack->capacity_)
        {
#ifdef STACK_DEBUG
            elem_t* temp = (elem_t*)calloc(1, (stack->capacity_ * stack->increase_factor_ + 2) * sizeof(elem_t));
            if (temp == NULL)
            {
                return;
            }

            free(stack->data_);
            stack->data_ = temp;
            *(elem_t*)stack->data_ = LEFT_CNRY;
            *((elem_t*)stack->data_ + (stack->capacity_ * stack->increase_factor_ + 1)) = RIGHT_CNRY;

#else

            elem_t* temp = (elem_t*)calloc(1, stack->capacity_ * stack->increase_factor_);
            if (temp == NULL)
            {
                return;
            }
#endif

            stack->capacity_ *= stack->increase_factor_;
            
#ifdef STACK_DEBUG
            stack->hash_summ = stackHash(stack);
#endif
        }
}

void shrinkToFit(Stack* stack)
{
    assert(stack != NULL);

#ifdef STACK_DEBUG
    if (stack->size_ * stack->increase_factor_ + 1 == stack->capacity_)
    {
        elem_t* temp = (elem_t*)calloc(1, (2 + stack->capacity_) / stack->increase_factor_ * sizeof(elem_t));
        if (temp == NULL)
        {
            return;
        }
        stack->data_ = temp;

        stack->capacity_ /= stack->increase_factor_;
        *(elem_t*)stack->data_ = LEFT_CNRY;
        *(elem_t*)(stack->data_ + sizeof(elem_t) + (stack->capacity_ / stack->increase_factor_) * sizeof(elem_t)) = RIGHT_CNRY;

        stack->hash_summ = stackHash(stack);
    }
#else
    elem_t* temp = (elem_t*)calloc(1, stack->capacity_ / stack->increase_factor_ * sizeof(elem_t));
    if (temp == NULL)
    {
        return;
    }
#endif
}

void push(Stack* stack, elem_t value)
{
    assert(stack != NULL);

    ASSERTOK(stack)

        stackNewSize(stack);

    ++stack->size_;
    stack->data_[stack->size_] = value;

#ifdef STACK_DEBUG  
    stack->hash_summ = stackHash(stack);
#endif

    ASSERTOK(stack)
}

void pop(Stack* stack)
{
    assert(stack != NULL);

    ASSERTOK(stack)

    --stack->size_;

#ifdef STACK_DEBUG
    stack->hash_summ = stackHash(stack);
#endif

    ASSERTOK(stack)
}

elem_t top(Stack* stack)
{
    assert(stack != NULL);

    ASSERTOK(stack)

        elem_t value = stack->data_[stack->size_];

    ASSERTOK(stack)

        return value;
}

void deleteStack(Stack* stack)
{
    assert(stack != NULL);

    ASSERTOK(stack)

        stack->data_ = NULL;
    stack->size_ = 0;
    stack->capacity_ = 0;
    free(stack);

#ifdef STACK_DEBUG
    stack->hash_summ = stackHash(stack);
#endif
}

void clearStack(Stack* stack)
{

    assert(stack != NULL);

    ASSERTOK(stack)

    stack->data_ = NULL;
    stack->capacity_ = 0;
    stack->size_ = 0;
    stack = NULL;

#ifdef STACK_DEBUG  
    stack->hash_summ = stackHash(stack);
#endif
}

STACK_ERROR stackOk(Stack* stack) {

    if (stack == NULL) {
        return CONSTRUCT_ERROR;
    }

#ifdef STACK_DEBUG
    if (stack->left_cnry_ != LEFT_CNRY || stack->right_cnry_ != RIGHT_CNRY) {
        return STRUCT_ERROR;
    }
#endif

#ifdef STACK_DEBUG
    if (hashOk(stack) != NO_ERROR) {
        return HASH_ERROR;
    }
#endif

#ifdef STACK_DEBUG
    if (stack->data_[0] != LEFT_CNRY ||
        stack->data_[stack->capacity_ + 1] != RIGHT_CNRY) {
        return data_ERROR;
    }
#endif

    if (stack->data_ == NULL) {
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

void stackDump(Stack* stack) {
    FILE* log_file = fopen("logfile.txt", "w");
    STACK_ERROR ERROR = stackOk(stack);
    switch (ERROR) {
    case (SIZE_ERROR): {
        fprintf(log_file, "Out of array!\n");
        fprintf(log_file, "Capacity:%d\n", stack->capacity_);
        fprintf(log_file, "Size:%d\n", stack->size_);
        for (int i = 1; i <= stack->size_; ++i) {
            fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->data_[i]);
        }

        break;
    }
    case (STACK_POINTER_ERROR): {
        fprintf(log_file, "Stack array error!\n");
        fprintf(log_file, "Capacity:%d\n", stack->capacity_);
        fprintf(log_file, "Size:%d\n", stack->size_);
        for (int i = 1; i <= stack->size_; ++i) {
            fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->data_[i]);
        }

        break;
    }
    case (POINTER_ERROR): {
        fprintf(log_file, "Memory array stack error!\n");
        fprintf(log_file, "Pointer is NULL:%p\n", stack->data_);
        fprintf(log_file, "Capacity:%d\n", stack->capacity_);
        fprintf(log_file, "Size:%d\n", stack->size_);
        for (int i = 1; i <= stack->size_; ++i) {
            fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->data_[i]);
        }

        break;
    }
    case (POP_ERROR): {
        fprintf(log_file, "Stack is empty, you cant pop element!\n");
        fprintf(log_file, "Capacity:%d\n", stack->capacity_);
        fprintf(log_file, "Size:%d\n", stack->size_);
        for (int i = 1; i <= stack->size_; ++i) {
            fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->data_[i]);
        }

        break;
    }
    case (CONSTRUCT_ERROR): {
        fprintf(log_file, "Stack construct error!\n");
        fprintf(log_file, "Pointer is NULL:%p\n", stack);
        fprintf(log_file, "Capacity:%d\n", stack->capacity_);
        fprintf(log_file, "Size:%d\n", stack->size_);
        for (int i = 1; i <= stack->size_; ++i) {
            fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->data_[i]);
        }

        break;
    }

#ifdef STACK_DEBUG
    case (HASH_ERROR): {
        fprintf(log_file, "Hash_error, you cant to change buffer!\n");
        fprintf(log_file, "Capacity:%d\n", stack->capacity_);
        fprintf(log_file, "Size:%d\n", stack->size_);
        for (int i = 1; i <= stack->size_; ++i) {
            fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->data_[i]);
        }

        break;
    }
#endif

#ifdef STACK_DEBUG
    case (STRUCT_ERROR): {
        fprintf(log_file, "You cant to change struct(stack)!\n");
        fprintf(log_file, "Right CNRY:%lf \n", stack->right_cnry_);
        fprintf(log_file, "Real CNRY:%lf", RIGHT_CNRY);
        fprintf(log_file, "Left CNRY:%lf \n", stack->left_cnry_);
        fprintf(log_file, "Real CNRY:%lf", LEFT_CNRY);
        fprintf(log_file, "Capacity:%d\n", stack->capacity_);
        fprintf(log_file, "Size:%d\n", stack->size_);
        for (int i = 1; i <= stack->size_; ++i) {
            fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->data_[i]);
        }

        break;
    }
#endif

#ifdef STACK_DEBUG
    case (data_ERROR): {
        fprintf(log_file, "You cant to change array(stack)!\n");
        fprintf(log_file, "Left  CNRY:%lf \n", stack->data_[0]);
        fprintf(log_file, "Real  CNRY:%lf\n", LEFT_CNRY);
        fprintf(log_file, "Right CNRY:%lf \n", stack->data_[stack->capacity_ + 1]);
        fprintf(log_file, "Real  CNRY:%lf \n", RIGHT_CNRY);
        fprintf(log_file, "Capacity:%d\n", stack->capacity_);
        fprintf(log_file, "Size:%d\n", stack->size_);
        for (int i = 1; i <= stack->size_; ++i) {
            fprintf(log_file, "array[%d] = %lf\n", i - 1, stack->data_[i]);
        }

        break;
    }
#endif
    }

    fclose(log_file);
}