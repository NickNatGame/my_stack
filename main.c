#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define SOFT_ASSERT(cond, ret)                                                                     \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "\nError: condition `%s` failed\n", #cond);                            \
            return (ret);                                                                          \
        }                                                                                          \
    } while (0)
enum errors_input{INPUT_OK = 0, INPUT_ERR = 1};

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

    free(stack.block);
    stack.pointer = NULL;
    return 0;
}