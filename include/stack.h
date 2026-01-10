#ifndef STACK_WORK_H
#define STACK_WORK_H

typedef unsigned long ul;

typedef enum {
    STACK_OK = 0,
    STACK_NULL_PTR = 1 << 0,
    STACK_OVERFLOW = 1 << 1,
    STACK_UNDERFLOW = 1 << 2,
    STACK_MEMORY_ERR = 1 << 3,
    STACK_CORRUPTED = 1 << 4
} stack_error;

typedef struct stack{
    void *block;
    int *pointer;
    int count_idx;
    int capasity;
    ul hash;
    stack_error error;
} my_stack;

void stack_initialize(my_stack *stack);
void push(my_stack *stack, int num);
void pop(my_stack *stack);
void stack_resize(my_stack *arr);
ul hash_create(my_stack *stack);
void stack_errs(my_stack *stack);
int check_canaries(my_stack *stack);
void dump(my_stack *stack);
#endif