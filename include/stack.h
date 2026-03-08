#ifndef STACK_WORK_H // NOTE: nice, thx for header-protectors
#define STACK_WORK_H

// FIXME: Please, never do things like this, it's considered a bad practice
// since: 1) you do that in header and force user of your library yo use UL name
// for unsigned long and nothing else 2) you make it complicated redefining
// things that already exists into short, non-readable names
typedef unsigned long UL;

typedef enum {
  STACK_OK = 0,
  STACK_NULL_PTR = 1 << 0,
  STACK_OVERFLOW = 1 << 1,
  STACK_UNDERFLOW = 1 << 2,
  STACK_MEMORY_ERR = 1 << 3,
  STACK_CORRUPTED = 1 << 4
} stack_error;

typedef struct stack {
  void *block;
  int *pointer;
  int count_idx;
  int capasity;
  UL hash;
  stack_error error;
} my_stack;

void stack_initialize(my_stack *stack);
void push(my_stack *stack, int num);
void pop(my_stack *stack);
void stack_resize(my_stack *arr);
UL hash_create(
    my_stack *stack); // TODO: Do user of you lib really needs this function?
                      // Consider moving it into .c file as a static one
void stack_errs(my_stack *stack);
int check_canaries(my_stack *stack);
void dump(my_stack *stack);
void stack_destroy(my_stack *stack);
#endif
