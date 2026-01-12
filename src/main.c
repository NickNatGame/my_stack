#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

#define SOFT_ASSERT(cond, ret)                                                 \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "\nError: condition `%s` failed\n", #cond);              \
      return (ret);                                                            \
    }                                                                          \
  } while (0)

int main() {
  int choice = 0;
  int num = 0;
  my_stack stack;
  stack_initialize(&stack);

  while (choice != 3) {
    printf("Choose:\n push: [1]\n pop: [2]\n");
    scanf("%d", &choice);
    if (choice == 1) {
      printf("Write number ");
      SOFT_ASSERT(scanf("%d", &num) == 1, EXIT_FAILURE);
      push(&stack, num);
    } else {
      pop(&stack);
    }
    dump(&stack);
  }

  stack_destroy(&stack);
  return 0;
}