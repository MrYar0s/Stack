#include "stack.h"

void StackCreate(Stack* tack){

    tack->minlen = 0;
    tack->maxlen = START;
    tack->len = 0;
    tack->hash = 0;

    tack->data = (element*) calloc(tack->maxlen * sizeof(element) + sizeof(CANARY_1) + sizeof(CANARY_2), sizeof(element));
    
    for (size_t i = 0; i < tack->maxlen; i++)
        tack->data[i + 1] = POISON;

    tack->data[0] = CANARY_1;
    tack->data[tack->maxlen + 1] = CANARY_2;

    VERIFY(tack);
}

void StackDiscard(Stack* tack){
    
    VERIFY(tack);

    for (size_t i = 0; i < tack->maxlen; ++i)
        tack->data[i + 1] = POISON;

    tack->minlen = 0;
    tack->maxlen = 0;
    tack->len = 0;
    tack->hash = 0;

}

element StackPop(Stack* tack){

    VERIFY(tack);

    tack->len--;

    if (tack->len < tack->minlen)
        MemShrink(tack);

    element n = tack->data[tack->len + 1];

    tack->data[tack->len + 1] = 0;

    tack->hash -= (size_t)(n * (tack->len + 1));

    VERIFY(tack);

    return n;
}

int StackPush(Stack* tack, element n){

    VERIFY(tack);

    tack->len++;

    if (tack->len >= tack->maxlen)
        MemExpand(tack);

    tack->data[tack->len] = n;

    tack->hash += (size_t)(n * (tack->len));

    VERIFY(tack);

    return 0;
}

void MemShrink(Stack* tack){

    VERIFY(tack);

    if (tack->minlen == 7)
        tack->minlen = 0;

    tack->maxlen = tack->maxlen / 2;
    tack->minlen = tack->minlen / 2;

    tack->data = (element*) realloc(tack->data, sizeof(CANARY_1) + sizeof(CANARY_2) + tack->maxlen * sizeof(element));

    tack->data[0] = CANARY_1;
    tack->data[tack->maxlen + 1] = CANARY_2;

    VERIFY(tack);

}

void MemExpand(Stack* tack){

    VERIFY(tack);

    tack->minlen = (3 * tack->maxlen) / 4;
    tack->maxlen = tack->maxlen * 2;

    tack->data = (element*) realloc(tack->data, sizeof(CANARY_1) + sizeof(CANARY_2) + (tack->maxlen * sizeof(element)));

    for (size_t i = tack->len; i <= tack->maxlen; ++i){
      tack->data[i] = POISON;
    }

    tack->data[0] = CANARY_1;
    tack->data[tack->maxlen + 1] = CANARY_2;

    VERIFY(tack);

}

int StackVerify(Stack* tack){

    if (tack == nullptr)
        return STACK_STRUCT_NULLPTR;

    if ((tack->data + sizeof(CANARY_1)) == nullptr)
        return STACK_NULLPTR;

    if (tack->minlen > tack->maxlen || tack->minlen > tack->len || tack->len > tack->maxlen)
        return INVALID_SIZE;

    if (tack->data == nullptr)
        return MEMORY_WASNT_SHRINK;

    if ((tack->data + sizeof(CANARY_1)) == nullptr)
        return MEMORY_WASNT_EXPAND;

    if (tack->data[tack->maxlen + 1] != CANARY_2)
        return STACK_OVERFLOW;

    if (tack->data[0] != CANARY_1)
        return STACK_UNDERFLOW;

    size_t sum = 0;

    for (size_t i = 1; i <= tack->len; i++)
        sum += (size_t)(tack->data[i] * (i));

    if (sum != tack->hash)
        return HASH_ERROR;

    if (tack->canary_struct_1 != CANARY_1)
        return CANARY_STRUCT_LEFT_ERROR;

    if (tack->canary_struct_2 != CANARY_2)
        return CANARY_STRUCT_RIGHT_ERROR;

    return 0;
}

int StackDump(Stack* tack, int err){

    FILE* code = fopen("dump.txt", "w");

    fprintf(code, "#####ERROR#####\n");

    fprintf(code, "Upper limit = %zu\n", tack->maxlen);

    fprintf(code, "Lower limit = %zu\n", tack->minlen);

    fprintf(code, "Number of elements = %zu\n", tack->len);

    switch(prog){

        case 1: fprintf(code, "First canary = %lg\n", tack->data[0]);
                fprintf(code, "Second canary = %lg\n", tack->data[tack->maxlen + 1]);
                break;

        case 2: fprintf(code, "First canary = %d\n", tack->data[0]);
                fprintf(code, "Second canary = %d\n", tack->data[tack->maxlen + 1]);
                break;

        case 3: fprintf(code, "First canary = %c\n", tack->data[0]);
                fprintf(code, "Second canary = %c\n", tack->data[tack->maxlen + 1]);
                break;

        default: fprintf(code, "First canary = NaN");
                 fprintf(code, "Second canary = NaN");
                 break;
    }

    fprintf(code, "Hash = %zu\n", tack->hash);

    fprintf(code, "Adress of start of the Stack = [%p]\n", (tack->data + sizeof(CANARY_1)));

    fprintf(code, "Some elements of Stack: \n");

    for (size_t i = 1; i <= tack->maxlen; i++){
        
        if (tack->data[i] == POISON)
            fprintf(code, "[%zu]: POISON\n", i);
        
        else {
            switch(prog){

            case 1: fprintf(code, "[%zu]: %lg\n", i, tack->data[i]);
                    break;

            case 2: fprintf(code, "[%zu]: %d\n", i, tack->data[i]);
                    break;

            case 3: fprintf(code, "[%zu]: %c\n", i, tack->data[i]);
                    break;

            default: fprintf(code, "undefined type");
                    break;
            }
        }
    }

    fprintf(code, "What happened: ");

    switch(err){

        case STACK_STRUCT_NULLPTR:  fprintf(code, "STACK STRUCT NULLPOINTER\n");
                                    break;

        case STACK_NULLPTR:         fprintf(code, "STACK NULLPOINTER\n");
                                    break;

        case INVALID_SIZE:          fprintf(code, "INVALID SIZE\n");
                                    break;

        case STACK_OVERFLOW:        fprintf(code, "STACK OVERFLOW\n");
                                    break;

        case STACK_UNDERFLOW:       fprintf(code, "STACK UNDERFLOW\n");
                                    break;

        case HASH_ERROR:            fprintf(code, "HASH ERROR\n");
                                    break;

        case MEMORY_WASNT_SHRINK:   fprintf(code, "MEMORY WASNT SHRINK\n");
                                    break;

        case MEMORY_WASNT_EXPAND:   fprintf(code, "MEMORY WASNT EXPAND\n");
                                    break;

        default:                    fprintf(code, "NOTHING\n");
                                    break;
    }

    fprintf(code, "CODE ERROR: %d\n", err);
    fclose(code);

    return 0;
}