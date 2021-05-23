#include <assert.h>
#include <cstdint>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------
//elem_t - stack content type
//-------------------
typedef int elem_t;

//-------------------
//Constants for stack debug
//-------------------
const elem_t LEFT_CNRY       = 0x212121;
const elem_t RIGHT_CNRY      = 0x121212;
const elem_t POISON_VALUE    = -1;
const int    INCREASE_FACTOR = 4;

//-------------------
//struct Stack -
//contains four main fields:
//
//array - array for contens
//size  - real size of stack 
//capacity - start const size 
//increase_factor - number for array reallocation
//-------------------
struct myStack 
{
 #ifdef STACK_DEBUG
   elem_t left_cnry_ = 0;
 #endif

//-----
  elem_t* array_           = NULL;
  int     size_            = 0;
  int     capacity_        = 0;
  int     increase_factor_ = 0;
//-----

 #ifdef STACK_DEBUG
   elem_t right_cnry_ = 0;
 #endif
 
 #ifdef STACK_DEBUG
   uint64_t hash_summ = 0;
 #endif
};

//-------------------
//Functions for stack
//-------------------
void     Push        (myStack* stack, int value);
void     Pop         (myStack* stack);
void     DeleteStack (myStack* stack);
void     ClearStack  (myStack* stack);
elem_t   Top         (myStack* stack);
int      StackSize   (myStack* stack);
uint64_t StackHash   (myStack* stack);
myStack* NewStack    (size_t start_size);

//-------------------
//Defines for stack debug
//-------------------
#ifdef STACK_DEBUG
#define ASSERTOK(stack) if (stackOk(stack) != NO_ERROR) { stackDump(stack); assert(!"OK");	}
#else 
#define ASSERTOK(stack) ;
#endif

//-------------------
//Number of stack error for
//stack debug
//-------------------
enum STACK_ERROR
{
  SIZE_ERROR,
  POINTER_ERROR,
  POP_ERROR,
  STACK_POINTER_ERROR,
  CONSTRUCT_ERROR,
  STRUCT_ERROR,
  ARRAY_ERROR,
  HASH_ERROR,
  NO_ERROR
};

//-------------------
//Stack debug functions
//-------------------
void        stackDump (myStack* stack);
STACK_ERROR stackOk   (myStack* stack);