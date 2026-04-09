#ifndef STACK_WORK_H
#define STACK_WORK_H

typedef enum {
  STACK_OK = 0,
  STACK_NULL_PTR = 1 << 0,
  STACK_OVERFLOW = 1 << 1,
  STACK_UNDERFLOW = 1 << 2,
  STACK_MEMORY_ERR = 1 << 3,
  STACK_CORRUPTED = 1 << 4
} stack_error;

#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME NULL
#endif
typedef struct stack {
  void *block;
  int *pointer;
  int size;
  int capacity;
  unsigned long hash;
  stack_error error;
} my_stack;

#define SOFT_ASSERT_STACK(cond, err_code, stack)                               \
  do {                                                                         \
    if (!(cond)) {                                                             \
      stack->error |= (err_code);                                              \
    }                                                                          \
  } while (0)

extern const unsigned int CANARY;

unsigned long hash_create(my_stack *stack);
void stack_initialize(my_stack *stack);
void push(my_stack *stack, int num);
void pop(my_stack *stack);
void stack_resize(my_stack *arr);
void stack_errs(my_stack *stack);
int check_canaries(my_stack *stack);
void dump(my_stack *stack);
void stack_destroy(my_stack *stack);

#endif