#include "stack.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned int CANARY = 0xDEADBEEF;
static int fl_err = 0;
#define SOFT_ASSERT_ERR(cond)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
      fl_err++;                                                                \
    }                                                                          \
  } while (0)

#define SOFT_ASSERT_EQ_ERR(a, b)                                               \
  do {                                                                         \
    int _a = (a);                                                              \
    int _b = (b);                                                              \
    if (_a != _b) {                                                            \
      fprintf(stderr, "FAIL %s:%d: %s == %s (got %d, expected %d)\n",          \
              __FILE__, __LINE__, #a, #b, _a, _b);                             \
      fl_err++;                                                                \
    }                                                                          \
  } while (0)

static int stack_check_init() {
  my_stack stack;
  stack_initialize(&stack);
  SOFT_ASSERT_ERR(stack.block != NULL);     /*  |                         */
  SOFT_ASSERT_ERR(stack.pointer != NULL);   /*  |                         */
  SOFT_ASSERT_ERR(stack.error == STACK_OK); /*  |    init values check    */
  SOFT_ASSERT_EQ_ERR(stack.capasity, 1);    /*  |                         */
  SOFT_ASSERT_EQ_ERR(stack.count_idx, -1);  /*  |                         */

  SOFT_ASSERT_ERR((char *)stack.pointer ==
                  (char *)stack.block + (int)sizeof(CANARY)); /* |                                      */
  SOFT_ASSERT_ERR(check_canaries(&stack) ==                   /* |                                      */
                  1);                                         /* |   check pointer, canaries and hash   */
  SOFT_ASSERT_ERR(stack.hash == hash_create(&stack));         /* |                                      */

  stack_destroy(&stack);
  return 0;
}

static int test_push_pop(void) {
  my_stack stack;
  stack_initialize(&stack);

  push(&stack, 10);                                     /* |                      */
  SOFT_ASSERT_EQ_ERR(stack.count_idx, 0);               /* |                      */
  SOFT_ASSERT_EQ_ERR(stack.pointer[0], 10);             /* |   check push basic   */
  SOFT_ASSERT_ERR(stack.hash == hash_create(&stack));   /* |                      */
  SOFT_ASSERT_ERR(check_canaries(&stack) == 1);         /* |                      */

  pop(&stack);                                          /* |                    */
  SOFT_ASSERT_EQ_ERR(stack.count_idx, -1);              /* |                    */
  SOFT_ASSERT_ERR((stack.error & STACK_UNDERFLOW) ==    /* |                    */
                  0);                                   /* |   check pop basic  */
  SOFT_ASSERT_ERR(stack.hash == hash_create(&stack));   /* |                    */
  SOFT_ASSERT_ERR(check_canaries(&stack) == 1);         /* |                    */

  stack_destroy(&stack);
  return 0;
}

static int test_resize(void) {
  my_stack stack;
  stack_initialize(&stack);

  push(&stack, 123);
  SOFT_ASSERT_EQ_ERR(stack.capasity, 1);
  SOFT_ASSERT_EQ_ERR(stack.pointer[0], 123);

  /* resize: 1 -> 2 */
  push(&stack, 228);
  SOFT_ASSERT_EQ_ERR(stack.capasity, 2);
  SOFT_ASSERT_EQ_ERR(stack.count_idx, 1);
  SOFT_ASSERT_EQ_ERR(stack.pointer[0], 123);
  SOFT_ASSERT_EQ_ERR(stack.pointer[1], 228);
  SOFT_ASSERT_ERR(stack.hash == hash_create(&stack));
  SOFT_ASSERT_ERR(check_canaries(&stack) == 1);

  /* resize: 2 -> 4 */
  push(&stack, 999);
  SOFT_ASSERT_EQ_ERR(stack.capasity, 4);
  SOFT_ASSERT_EQ_ERR(stack.count_idx, 2);
  SOFT_ASSERT_EQ_ERR(stack.pointer[0], 123);
  SOFT_ASSERT_EQ_ERR(stack.pointer[1], 228);
  SOFT_ASSERT_EQ_ERR(stack.pointer[2], 999);
  SOFT_ASSERT_ERR(stack.hash == hash_create(&stack));
  SOFT_ASSERT_ERR(check_canaries(&stack) == 1);

  stack_destroy(&stack);
  return 0;
}

static void test_many_pushes_and_pops(void) {
  my_stack stack;
  stack_initialize(&stack);

  const int N = 100;
  int capas_real = 1;
  for (int i = 0; i < N; i++) {
    if (i == capas_real)
      capas_real *= 2;
    push(&stack, i);
  }

  SOFT_ASSERT_EQ_ERR(stack.count_idx, N - 1);
  SOFT_ASSERT_EQ_ERR(stack.capasity, capas_real);

  SOFT_ASSERT_EQ_ERR(stack.pointer[0], 0);      /* |                 */
  SOFT_ASSERT_EQ_ERR(stack.pointer[50], 50);    /* | can be changed  */
  SOFT_ASSERT_EQ_ERR(stack.pointer[99], 99);    /* |                 */

  SOFT_ASSERT_ERR(stack.hash == hash_create(&stack));
  SOFT_ASSERT_ERR(check_canaries(&stack) == 1);

  for (int i = 0; i < 10; i++)
    pop(&stack);
  SOFT_ASSERT_EQ_ERR(stack.count_idx, N - 11);
  SOFT_ASSERT_ERR(stack.hash == hash_create(&stack));
  SOFT_ASSERT_ERR(check_canaries(&stack) == 1);

  stack_destroy(&stack);
}

int main(void) {
  stack_check_init();
  test_push_pop();
  test_resize();
  test_many_pushes_and_pops();

  if (fl_err == 0) {
    puts("OK: all tests passed");
    return 0;
  } else {
    fprintf(stderr, "FAILED: %d checks\n", fl_err);
    return 1;
  }
}