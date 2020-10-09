#include "stack.h"

int main(){

    Stack steck;

    StackCreate(&steck);
    
    StackPush(&steck, 2);

    for(size_t p = 0; p < steck.len; p++){
        printf("\n%f ", *(steck.data + sizeof(CANARY_1) + sizeof(element) * p));
    }

    StackDiscard(&steck);
    
    return 0;
}