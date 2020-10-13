#include "stack.h"

int main(){

    Stack steck;

    StackCreate(&steck);

    for(size_t i = 1; i <= 100; ++i){
        StackPush(&steck, i);
    }

    StackDump(&steck, 0);

    //StackDiscard(&steck);
    
    return 0;
}