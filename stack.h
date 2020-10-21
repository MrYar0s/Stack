#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define START 10
#ifndef VERIFY
#define VERIFY(a) if(StackVerify(a) != 0)                                                       \
                    {                                                                           \
                        FILE* log = fopen("log.txt", "w");                                      \
                        fprintf(log, "ERROR: line %d function %s\n", __LINE__, __FUNCTION__);   \
                        StackDump(a, StackVerify(a));                                           \
                        exit(-1);                                                               \
                    }
#endif
#define CHAR_T

#ifdef DOUBLE_T
    typedef double element;
    const int prog = 1;
    const double POISON = 1.2;
    const element CANARY_1 = 0xDED0BED;
    const element CANARY_2 = 0xD0C;
#endif

#ifdef INT_T
    typedef int element;
    const int prog = 2;
    const int POISON = 1357;
    const element CANARY_1 = 0xDED0BED;
    const element CANARY_2 = 0xD0C;
#endif

#ifdef CHAR_T
    typedef char element;
    const int prog = 3;
    const char POISON = 'c';
    const element CANARY_1 = 'D';
    const element CANARY_2 = 'B';
#endif

struct Stack{
    size_t canary_struct_1 = CANARY_1;
    
    size_t minlen = 0;
    size_t maxlen = START;
    size_t len = 0;
    size_t hash = 0;
    element* data = NULL;
    
    size_t canary_struct_2 = CANARY_2;
};

enum error{
    STACK_STRUCT_NULLPTR = 10,
    STACK_NULLPTR = 20,
    INVALID_SIZE = 30,
    STACK_OVERFLOW = 40,
    STACK_UNDERFLOW = 50,
    MEMORY_WASNT_SHRINK = 60,
    MEMORY_WASNT_EXPAND = 70,
    HASH_ERROR = 80,
    CANARY_STRUCT_LEFT_ERROR = 90,
    CANARY_STRUCT_RIGHT_ERROR = 100
};

void StackCreate(Stack* tack);

void StackDiscard(Stack* tack);

int StackPush(Stack* tack, element n);

element StackPop(Stack* tack);

void MemExpand(Stack* tack);

void MemShrink(Stack* tack);

int StackVerify(Stack* tack);

int StackDump(Stack* tack, int err);