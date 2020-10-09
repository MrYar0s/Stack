#include "stack.h"

void StackCreate(Stack* tack){

    tack->minlen = 0;
    tack->maxlen = START;
    tack->len = 0;
    tack->hash = 0;

    tack->data = (element*) calloc(START * sizeof(element) + sizeof(CANARY_1) + sizeof(CANARY_2), sizeof(element));
    *(tack->data) = CANARY_1;
    *(tack->data + sizeof(CANARY_1) + START * sizeof(element)) = CANARY_2;

    for(size_t i = 0; i < START; i++)
        *(tack->data + sizeof(CANARY_1) + sizeof(element) * i) = POISON;

    VERIFY;
}

void StackDiscard(Stack* tack){
    
    VERIFY;

    for(size_t i = 0; i < tack->maxlen; ++i)
        *(tack->data + sizeof(CANARY_1) + i * sizeof(element)) = POISON;

    tack->minlen = 0;
    tack->maxlen = 0;
    tack->len = 0;
    tack->hash = 0;

}

element StackPop(Stack* tack){

    VERIFY;

    tack->len--;

    if(tack->len < tack->minlen)
        MemShrink(tack);


    element n = *(tack->data + sizeof(CANARY_1) + sizeof(element) * tack->len);

    *(tack->data + sizeof(CANARY_1) + sizeof(element) * tack->len) = 0;

    tack->hash -= n * (tack->len + 1);

    VERIFY;

    return n;
}

int StackPush(Stack* tack, element n){

    VERIFY;

    tack->len++;

    if(tack->len > tack->maxlen)
        MemExpand(tack);

    *(tack->data + sizeof(CANARY_1) + sizeof(element) * (tack->len - 1)) = n;

    tack->hash += *(tack->data + sizeof(CANARY_1) + sizeof(element) * (tack->len - 1)) * tack->len;

    VERIFY;

    return 0;
}

void MemShrink(Stack* tack){

    if(tack->minlen == 7)
        tack->minlen = 0;

    tack->maxlen = tack->maxlen / 2;
    tack->minlen = tack->minlen / 2;

    tack->data = (element*) realloc(tack->data, sizeof(CANARY_1) + sizeof(CANARY_2) + tack->maxlen * sizeof(element));

    for(size_t i = tack->len; i < tack->maxlen; ++i){
        *(tack->data + sizeof(CANARY_1) + i * sizeof(element)) = POISON;
    }

    if(tack->data == nullptr){
        StackDump(tack, MEMORY_WASNT_SHRINK);
        exit(-1);
    }

    *(tack->data) = CANARY_1;
    *(tack->data + sizeof(CANARY_1) + tack->maxlen * sizeof(element));

}

void MemExpand(Stack* tack){

    tack->minlen = (3 * tack->maxlen) / 4;
    tack->maxlen = tack->maxlen * 2;

    tack->data = (element*) realloc(tack->data, sizeof(CANARY_1) + sizeof(CANARY_2) + tack->maxlen * sizeof(element));

    if((tack->data + sizeof(CANARY_1)) == nullptr){
        StackDump(tack, MEMORY_WASNT_EXPAND);
        exit(-1);
    }

//    tack->data = (element*) realloc(tack->data, tack->maxlen * sizeof(element));

    *(tack->data) = CANARY_1;
    *(tack->data + sizeof(CANARY_1) + tack->maxlen * sizeof(element)) = CANARY_2;

}

int StackVerify(Stack* tack){

    if(tack == nullptr)
        return STACK_STRUCT_NULLPTR;

    if((tack->data + sizeof(CANARY_1)) == nullptr)
        return STACK_NULLPTR;

    if(tack->minlen > tack->maxlen || tack->minlen > tack->len || tack->len > tack->maxlen)
        return INVALID_SIZE;

    if(*(tack->data + sizeof(CANARY_1) + tack->maxlen * sizeof(element)) != CANARY_2)
        return STACK_OVERFLOW;

    if(*(tack->data) != CANARY_1)
        return STACK_UNDERFLOW;

    size_t sum = 0;

    for(size_t i = 0; i < tack->len; i++)
        sum += *(tack->data + sizeof(CANARY_1) + sizeof(element) * i) * (i + 1);

    if(sum != tack->hash)
        return HASH_ERROR;

    return 0;
}

int StackDump(Stack* tack, int err){

    FILE* code = fopen("dump.txt", "w");

    fprintf(code, "#####ERROR#####\n");

    fprintf(code, "Upper limit = %zu\n", tack->maxlen);

    fprintf(code, "Lower limit = %zu\n", tack->minlen);

    fprintf(code, "Number of elements = %zu\n", tack->len);

    fprintf(code, "Hash = %zu\n", tack->hash);

    fprintf(code, "Adress of start of the Stack = [%p]\n", (tack->data + sizeof(CANARY_1)));

    fprintf(code, "Some elements of Stack: \n");

    if(*(tack->data + sizeof(CANARY_1)) == POISON)
        fprintf(code, "[1]: POISON\n");
    else
        fprintf(code, "[1]: %lg\n", *(tack->data + sizeof(CANARY_1)));

    if(*(tack->data + sizeof(CANARY_1) + sizeof(element)) == POISON)
        fprintf(code, "[2]: POISON\n");
    else
        fprintf(code, "[2]: %lg\n", *(tack->data + sizeof(CANARY_1) + sizeof(element) * tack->len));

    if(*(tack->data + sizeof(CANARY_1) + 2 * sizeof(element)) == POISON)
        fprintf(code, "[3]: POISON\n");
    else
        fprintf(code, "[3]: %lg\n", *(tack->data + sizeof(CANARY_1) + 2 * sizeof(element) * tack->len));

    if(*(tack->data + sizeof(CANARY_1) + 3 * sizeof(element)) == POISON)
        fprintf(code, "[4]: POISON\n");
    else
        fprintf(code, "[4]: %lg\n", 
                *(tack->data + sizeof(CANARY_1) + 3 * sizeof(element) * tack->len));

    fprintf(code, ".........\n");

    fprintf(code, "[i]: data[i]\n");

    fprintf(code, ".........\n");

    if(tack->len > 3){

        fprintf(code, "[%zu]: %lg\n", tack->len, *(tack->data + sizeof(CANARY_1) + sizeof(element) * (tack->len - 1)));

            if(*(tack->data + sizeof(CANARY_1) + sizeof(element) * (tack->len - 1)) == POISON)
                fprintf(code, "[%zu]: POISON\n", tack->len);
            else
                fprintf(code, "[%zu]: %lg\n", tack->len, *(tack->data + sizeof(CANARY_1) + sizeof(element) * (tack->len - 1)));
    }

    fprintf(code, "What happened: ");

    switch(err){

        case STACK_STRUCT_NULLPTR:  fprintf(code, "STACK STRUCT NULLPOINTER\n");
                                    fprintf(code, "CODE ERROR: %d\n", err);
                                    fclose(code);
                                    return STACK_NULLPTR;

        case STACK_NULLPTR:         fprintf(code, "STACK NULLPOINTER\n");
                                    fprintf(code, "CODE ERROR: %d\n", err);
                                    fclose(code);
                                    return STACK_NULLPTR;

        case INVALID_SIZE:          fprintf(code, "INVALID SIZE\n");
                                    fprintf(code, "CODE ERROR: %d\n", err);
                                    fclose(code);
                                    return INVALID_SIZE;

        case STACK_OVERFLOW:        fprintf(code, "STACK OVERFLOW\n");
                                    fprintf(code, "CODE ERROR: %d\n", err);
                                    fclose(code);
                                    return STACK_OVERFLOW;

        case STACK_UNDERFLOW:       fprintf(code, "STACK UNDERFLOW\n");
                                    fprintf(code, "CODE ERROR: %d\n", err);
                                    fclose(code);
                                    return STACK_UNDERFLOW;

        case HASH_ERROR:            fprintf(code, "HASH ERROR\n");
                                    fprintf(code, "CODE ERROR: %d\n", err);
                                    fclose(code);
                                    return HASH_ERROR;

        case MEMORY_WASNT_SHRINK:   fprintf(code, "MEMORY WASNT SHRINK\n");
                                    fprintf(code, "CODE ERROR: %d\n", err);
                                    fclose(code);
                                    return MEMORY_WASNT_SHRINK;

        case MEMORY_WASNT_EXPAND:   fprintf(code, "MEMORY WASNT EXPAND\n");
                                    fprintf(code, "CODE ERROR: %d\n", err);
                                    fclose(code);
                                    return MEMORY_WASNT_EXPAND;

        default:                    fprintf(code, "NOTHING\n");
                                    fprintf(code, "THERE IS NO ERROR\n");
                                    break;
    }

    fclose(code);

    return 0;
}