#ifndef STACK_WORK_H
#define STACK_WORK_H

#include <stddef.h>

#define DEF_ERROR(CODE, VALUE, STR) CODE = VALUE,
typedef enum {
  #include "error.def"
} stack_error;
#undef DEF_ERROR

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

#ifndef NDEBUG
#define SOFT_ASSERT_STACK(cond, err_code, stack)                               \
  do {                                                                         \
    if (!(cond) && (stack) != NULL) {                                          \
      (stack)->error |= (err_code);                                            \
    }                                                                          \
  } while (0)
#else
#define SOFT_ASSERT_STACK(cond, err_code, stack)                               \
  do {                                                                         \
    (void)(cond);                                                              \
    (void)(err_code);                                                          \
    (void)(stack);                                                             \
  } while (0)
#endif

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
