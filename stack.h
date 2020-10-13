#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define START 10
#define VERIFY if(StackVerify(tack) != 0){StackDump(tack, StackVerify(tack)); exit(-1);}
#define POISON 1357

typedef int element;

struct Stack{
    size_t minlen = 0;
    size_t maxlen = START;
    size_t len = 0;
    size_t hash = 0;
    element* data = NULL;
};

enum error{
    STACK_STRUCT_NULLPTR = 10,
    STACK_NULLPTR = 20,
    INVALID_SIZE = 30,
    STACK_OVERFLOW = 40,
    STACK_UNDERFLOW = 50,
    MEMORY_WASNT_SHRINK = 60,
    MEMORY_WASNT_EXPAND = 70,
    HASH_ERROR = 80
};

void StackCreate(Stack* tack);

void StackDiscard(Stack* tack);

int StackPush(Stack* tack, element n);

element StackPop(Stack* tack);

void MemExpand(Stack* tack);

void MemShrink(Stack* tack);

int StackVerify(Stack* tack);

int StackDump(Stack* tack, int err);
