#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

// FIXME: I believe, it is not the obly place you need this macro, so move it
// into header
//        also, you don't need this additional check in production-ready build,
//        sice they would slow down your exection, so make them turned off in
//        production build (see CmakeLists.txt comments)
#define SOFT_ASSERT(cond, ret)                                                 \
  do {                                                                         \
    if (!(cond)) {                                                             \
      /* TODO: btw, it's not always good to print into stderr, what if I would \
       * like to save logs into file? so make fprintf destionation as LOG_FILE \
       * define which is stderr by default, but may be set into something      \
       * different with cmake's -DLOG_FILE_NAME=... definition*/               \
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

    // FIXME: [CRITICAL]: What if user writes down something else like
    // `aboba` or `4`? There would be an pop from an empty stack then
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
