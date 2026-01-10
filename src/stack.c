#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "stack.h"

#define SOFT_ASSERT_STACK(cond, err_code, stack) \
    do { \
        if (!(cond)) { \
            stack->error |= (err_code); \
        } \
    } while (0)

static const unsigned int CANARY = 0xDEADBEEF;
static const int mult = 2;
static void write_canaries(my_stack *stack);


ul hash_create(my_stack *stack){
    ul hash = 0x16032007;
    ul hash_prime = 0x01000193;
    for(int elem = 0; elem <= stack->count_idx; elem++){
        for(int i = 0; i < (int)sizeof(int)*8;i++){
            hash ^= (stack->pointer[elem] >> i) & 1;
            hash *= hash_prime;
        }
    }
    return hash;
}

static void write_canaries(my_stack *stack){
    memcpy((char*)stack->block, &CANARY, sizeof(CANARY));
    memcpy((char*)stack->block + sizeof(CANARY) + stack->capasity * sizeof(int), &CANARY, sizeof(CANARY));
}

int check_canaries(my_stack *stack){
    ul left_can = 0;
    ul right_can = 0;

    memcpy(&left_can, (char*)stack->block, sizeof(CANARY));
    memcpy(&right_can, (char*)stack->block + stack->capasity * sizeof(int) + sizeof(CANARY), sizeof(CANARY));

    return (left_can == CANARY && right_can == CANARY);
}

void stack_initialize(my_stack *stack){
    int total_bytes = 0;
    void *n_block = NULL;
    stack->capasity = 1;
    stack->error = STACK_OK;
    stack->count_idx = -1; 

    total_bytes = sizeof(CANARY) + stack->capasity * sizeof(int) + sizeof(CANARY);
    n_block = calloc(1, total_bytes);

    stack->block = n_block;
    stack->pointer = (int*)((char*)n_block + sizeof(CANARY)); 
    SOFT_ASSERT_STACK(stack->pointer != NULL, STACK_MEMORY_ERR, stack);

    write_canaries(stack);
    stack->hash = hash_create(stack);
}

void stack_resize(my_stack *stack){
    SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
    int new_total_bytes = 0;
    void *new_n_block = NULL;

    stack->capasity *= mult;
    new_total_bytes = sizeof(CANARY) + stack->capasity * sizeof(int) + sizeof(CANARY);
    new_n_block = realloc((char*)stack->block, new_total_bytes);
    if(new_n_block == NULL){
        stack-> error |= STACK_MEMORY_ERR;
    }

    stack->block = new_n_block;
    stack->pointer = (int *)((char*) new_n_block + sizeof(CANARY));

    SOFT_ASSERT_STACK(stack->pointer != NULL, STACK_MEMORY_ERR, stack);

    write_canaries(stack);
}

void push(my_stack *stack, int num){
    SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);

    stack->count_idx += 1;
    if(stack->count_idx >= stack->capasity) {
        stack_resize(stack);

        SOFT_ASSERT_STACK(stack->error != STACK_OK ? 0 : 1, STACK_CORRUPTED, stack);
    }
    stack->pointer[stack->count_idx] = num;
    stack->hash = hash_create(stack);
    write_canaries(stack);
}

void pop(my_stack *stack){
    SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);

    SOFT_ASSERT_STACK(stack->count_idx >= 0, STACK_UNDERFLOW, stack);

    if(stack->count_idx > -1){
        stack->count_idx -= 1;
    }
    stack->hash = hash_create(stack);
    write_canaries(stack);
}

void stack_errs(my_stack *stack){
    if(stack->error == STACK_OK){
        printf("No errors occurred\n");
        return;
    }

    printf("All stack errors:\n");
    if(stack->error & STACK_NULL_PTR){
        printf("Null pointer at stack\n");
    }
    if(stack->error & STACK_MEMORY_ERR){
        printf("Stack allocation err\n");
    }
    if(stack->error & STACK_OVERFLOW){
        printf("Stack oveflow - max capasity\n");
    }
    if(stack->error & STACK_UNDERFLOW){
        printf("Stack underflow - pop from empty stack\n");
    }
    if(stack->error & STACK_CORRUPTED){
        printf("Smth wrong with your stack...\n");
    }

    stack->error = STACK_OK;
}

#ifdef LOG_LEVEL_FULL
void dump(my_stack *stack){
    printf("\nLast element: stack[%d] %d\n", stack->count_idx, stack->pointer[stack->count_idx]);
    printf("All elements: \n");
    for(int i = stack->count_idx; i >= 0; i--){
        printf("stack[%d] %d\n",i, stack->pointer[i]);
    }
    printf("Capacity: %d\n", stack->capasity);
    printf("Hash: %lu\nCheck: %lu\n\n", stack->hash, hash_create(stack));
    if(check_canaries(stack)){
        printf("All fine with canaries\n");
    }
    else{
        printf("Canaries check fail - memory corrupted\n");
        stack->error |= STACK_CORRUPTED;
    }
    stack_errs(stack);
}

#elif LOG_LEVEL_MIDDLE
int dump(my_stack *stack){
    printf("Last element: stack[%d] %d\n", stack->count_idx, stack->pointer[stack->count_idx]);

    printf("All elements: \n");
    for(int i = stack->count_idx; i >= 0; i--){
        printf("stack[%d] %d\n",i, stack->pointer[i]);
    }
    printf("Capacity: %d\n", (stack->count_idx+1)*sizeof(int));
}

#elif LOG_LEVEL_NONE
void dump(my_stack *stack) {
    printf("Last element: stack[%d] %d\n", stack->count_idx, stack->pointer[stack->count_idx]); 
    printf("All elements: \n");
    for(int i = stack->count_idx; i >= 0; i--){
        printf("stack[%d] %d\n",i, stack->pointer[i]);
    }
}
#endif