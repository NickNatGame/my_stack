#include <stdio.h>
#include <stdlib.h>


typedef enum {
    STACK_OK = 0,
    STACK_NULL_PTR = 1 << 0,
    STACK_OVERFLOW = 1 << 1,
    STACK_UNDERFLOW = 1 << 2,
    STACK_MEMORY_ERR = 1 << 3,
    STACK_CORRUPTED = 1 << 4
} stack_error;
typedef unsigned long ul;

struct stack{
    int *pointer;
    int count_idx;
    int capasity;
    ul hash;
    stack_error error;
};
typedef struct stack my_stack;

#define SOFT_ASSERT_STACK(cond, err_code, stack) \
    do { \
        if ((cond)) { \
            stack->error |= (err_code); \                                                              
        } \
    } while (0)

#define SOFT_ASSERT(cond, ret)                                                                     \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "\nError: condition `%s` failed\n", #cond);                            \
            return (ret);                                                                          \
        }                                                                                          \
    } while (0)

const int CANARY = 0xDEADBEEF;
const int init_capacity = 2;
enum errors_input{INPUT_OK = 0, INPUT_ERR = 1};

void stack_initialize(my_stack *stack);
void push(my_stack *stack, int num);
void pop(my_stack *stack);
void stack_resize(my_stack *arr);
void dump(my_stack *stack);
ul hash_create(my_stack *stack);
void stack_errs(my_stack *stack);

int main(){ 
    int choice = 0;
    int num = 0;
    my_stack stack;
    stack_initialize(&stack);
    
    while(choice != 3){
        printf("Choose:\n push: [1]\n pop: [2]\n");
        scanf("%d",&choice);
        if(choice == 1){
            printf("Write number ");
            SOFT_ASSERT(scanf("%d", &num) == 1, INPUT_ERR);
            push(&stack, num);
        }
        else{
            pop(&stack);
        }
        dump(&stack);
    }
    return 0;
}

ul hash_create(my_stack *stack){
    ul hash = 0x16032007;
    ul hash_prime = 0x01000193;
    for(int elem = 0; elem <= stack->count_idx; elem++){
        for(int i = 0; i < sizeof(int)*8;i++){
            hash ^= (stack->pointer[elem] >> i) & 1;
            hash *= hash_prime;
        }
    }
    return hash;
}

void stack_initialize(my_stack *stack){ 
    SOFT_ASSERT_STACK(stack == NULL, STACK_NULL_PTR, stack);

    stack->pointer = (int *)calloc(init_capacity, sizeof(int));

    SOFT_ASSERT_STACK(stack->pointer == NULL, STACK_MEMORY_ERR, stack);

    stack->capasity = init_capacity;
    stack->count_idx = -1; 
    stack->hash = hash_create(stack);
    /*
    add canary on the start and end of the dinamic memory
    */
}

void stack_resize(my_stack *stack){
    SOFT_ASSERT_STACK(stack == NULL, STACK_NULL_PTR, stack);
    stack->capasity += 1;
    stack->pointer = (int *)realloc(stack->pointer, stack->capasity * sizeof(int) + 2 * sizeof(CANARY));
    SOFT_ASSERT_STACK(stack->pointer == NULL, STACK_MEMORY_ERR, stack);
    /*
    add canary on the start and end of the dinamic memory (?) and mb redo with calloc + memcpy
    */
}

void push(my_stack *stack, int num){
    SOFT_ASSERT_STACK(stack == NULL, STACK_NULL_PTR, stack);

    stack->count_idx += 1;
    if(stack->count_idx >= stack->capasity) {
        stack_resize(stack);

        SOFT_ASSERT_STACK(stack->error == STACK_OK ? 1 : 0, STACK_CORRUPTED, stack);
    }
    stack->pointer[stack->count_idx] = num;
    stack->hash = hash_create(stack);
    //add check of the canaries and hesh and if false - failure to dump
}
void pop(my_stack *stack){
    SOFT_ASSERT_STACK(stack == NULL, STACK_NULL_PTR, stack);

    SOFT_ASSERT_STACK(stack->count_idx == -1, STACK_UNDERFLOW, stack);

    if(stack->count_idx > -1){
        stack->count_idx -= 1;
    }
    stack->hash = hash_create(stack);
    //add check of the canaries and hesh and if false - failure to dump
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
void dump() {
    printf("Last element: stack[%d] %d\n", stack->count_idx, stack->pointer[stack->count_idx]); 
    printf("All elements: \n");
    for(int i = stack->count_idx; i >= 0; i--){
        printf("stack[%d] %d\n",i, stack->pointer[i]);
    }
}
#endif