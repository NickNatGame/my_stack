#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

const unsigned int CANARY = 0xDEADBEEF;
static const int mult = 2;
static void write_canaries(my_stack *stack);

static FILE *get_log_file(void)
{
  static FILE *log_file = NULL;
  static int initialized = 0;

  if (!initialized)
  {
    initialized = 1;

    if (LOG_FILE_NAME != NULL)
    {
      log_file = fopen(LOG_FILE_NAME, "a");
      if (log_file == NULL)
      {
        fprintf(stderr, "Failed to open log file '%s', using stderr\n",
                LOG_FILE_NAME);
        log_file = stderr;
      }
    }
    else
    {
      log_file = stderr;
    }
  }

  return log_file;
};

#define SOFT_ASSERT(cond, ret)                                            \
  do                                                                      \
  {                                                                       \
    if (!(cond))                                                          \
    {                                                                     \
      fprintf(get_log_file(), "\nError: condition `%s` failed\n", #cond); \
      return (ret);                                                       \
    }                                                                     \
  } while (0)

void stack_destroy(my_stack *stack)
{
  free(stack->block);
  stack->block = NULL;
  stack->pointer = NULL;
}

static void
write_canaries(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  SOFT_ASSERT_STACK(stack->block != NULL, STACK_NULL_PTR, stack);

  memcpy((char *)stack->block, &CANARY, sizeof(CANARY));

  memcpy((char *)stack->block + sizeof(CANARY) + stack->capacity * sizeof(int),
         &CANARY, sizeof(CANARY));
}

int check_canaries(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  SOFT_ASSERT_STACK(stack->block != NULL, STACK_NULL_PTR, stack);

  const unsigned int *left_can_ptr = (const unsigned int *)stack->block;
  const unsigned int *right_can_ptr =
      (const unsigned int *)((const char *)stack->block +
                             sizeof(CANARY) + stack->capacity * sizeof(int));

  return (*left_can_ptr == CANARY && *right_can_ptr == CANARY);
}

void stack_initialize(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);

  int total_bytes = 0;
  void *n_block = NULL;
  stack->capacity = 1;
  stack->error = STACK_OK;
  stack->size = 0;

  total_bytes = sizeof(CANARY) + stack->capacity * sizeof(int) + sizeof(CANARY);
  n_block = calloc(1, total_bytes);
  SOFT_ASSERT_STACK(n_block != NULL, STACK_MEMORY_ERR, stack);

  stack->block = n_block;
  stack->pointer = (int *)((char *)n_block + sizeof(CANARY));

  write_canaries(stack);
  stack->hash = hash_create(stack);
}

void stack_resize(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  SOFT_ASSERT_STACK(stack->block != NULL, STACK_NULL_PTR, stack);

  int new_capacity = 0;
  int new_total_bytes = 0;
  void *new_n_block = NULL;

  new_capacity = stack->capacity;
  if (stack->size >= stack->capacity)
  {
    new_capacity *= mult;
  }
  else if (stack->size <= stack->capacity / mult && stack->capacity > 1)
  {
    new_capacity /= mult;
    if (new_capacity < 1)
    {
      new_capacity = 1;
    }
  }
  else
  {
    return;
  }

  new_total_bytes =
      sizeof(CANARY) + new_capacity * sizeof(int) + sizeof(CANARY);

  new_n_block = realloc(stack->block, new_total_bytes);
  SOFT_ASSERT_STACK(new_n_block != NULL, STACK_MEMORY_ERR, stack);

  stack->block = new_n_block;
  stack->capacity = new_capacity;
  stack->pointer = (int *)((char *)stack->block + sizeof(CANARY));

  write_canaries(stack);
}

void push(my_stack *stack, int num)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);

  if (stack->size >= stack->capacity)
  {
    stack_resize(stack);

    SOFT_ASSERT_STACK(stack->error != STACK_OK ? 0 : 1, STACK_CORRUPTED, stack);
  }
  stack->pointer[stack->size] = num;
  stack->size += 1;

  stack->hash = hash_create(stack);
  write_canaries(stack);
}

void pop(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  SOFT_ASSERT_STACK(stack->size > 0, STACK_UNDERFLOW, stack);

  if (stack->size > 0)
  {
    stack->size -= 1;
  }

  if (stack->size <= stack->capacity / mult && stack->capacity > 1)
  {
    stack_resize(stack);
  }

  stack->hash = hash_create(stack);
  write_canaries(stack);
}

unsigned long hash_create(my_stack *stack)
{
  unsigned long hash = 0x16032007;
  const unsigned long hash_prime = 0x01000193;
  for (int elem = 0; elem < stack->size; elem++)
  {
    for (int i = 0; i < (int)sizeof(int) * 8; i++)
    {
      hash ^= (stack->pointer[elem] >> i) & 1;
      hash *= hash_prime;
    }
  }
  return hash;
}

void stack_errs(my_stack *stack)
{
  FILE *out = get_log_file();

  if (stack->error == STACK_OK)
  {
    printf("No errors occurred\n");
    return;
  }

  fprintf(out, "All stack errors:\n");
  if (stack->error & STACK_NULL_PTR)
  {
    fprintf(out, "Null pointer at stack\n");
  }
  if (stack->error & STACK_MEMORY_ERR)
  {
    fprintf(out, "Stack allocation err\n");
  }
  if (stack->error & STACK_OVERFLOW)
  {
    fprintf(out, "Stack overflow - max capacity\n");
  }
  if (stack->error & STACK_UNDERFLOW)
  {
    fprintf(out, "Stack underflow - pop from empty stack\n");
  }
  if (stack->error & STACK_CORRUPTED)
  {
    fprintf(out, "Smth wrong with your stack...\n");
  }

  stack->error = STACK_OK;
}

#ifdef LOG_LEVEL_FULL
void dump(my_stack *stack)
{
  if (stack->size > 0)
  {
    printf("\nLast element: stack[%d] %d\n", stack->size - 1,
           stack->pointer[stack->size - 1]);
  }
  else
  {
    printf("\nStack is empty\n");
  }
  printf("All elements: \n");
  for (int i = stack->size - 1; i >= 0; i--)
  {
    printf("stack[%d] %d\n", i, stack->pointer[i]);
  }
  printf("Capacity: %d\n", stack->capacity);
  printf("Hash: %lu\nCheck: %lu\n\n", stack->hash, hash_create(stack));
  if (check_canaries(stack))
  {
    printf("All fine with canaries\n");
  }
  else
  {
    printf("Canaries check fail - memory corrupted\n");
    stack->error |= STACK_CORRUPTED;
  }
  stack_errs(stack);
}

#elif LOG_LEVEL_MIDDLE
void dump(my_stack *stack)
{
  if (stack->size > 0)
  {
    printf("\nLast element: stack[%d] %d\n", stack->size - 1,
           stack->pointer[stack->size - 1]);
  }
  else
  {
    printf("\nStack is empty\n");
  }

  printf("All elements: \n");
  for (int i = stack->size - 1; i >= 0; i--)
  {
    printf("stack[%d] %d\n", i, stack->pointer[i]);
  }
  printf("Capacity: %d\n", (stack->size + 1) * sizeof(int));
}

#elif LOG_LEVEL_NONE
void dump(my_stack *stack)
{
  if (stack->size > 0)
  {
    printf("\nLast element: stack[%d] %d\n", stack->size - 1,
           stack->pointer[stack->size - 1]);
  }
  else
  {
    printf("\nStack is empty\n");
  }
  printf("All elements: \n");
  for (int i = stack->size - 1; i >= 0; i--)
  {
    printf("stack[%d] %d\n", i, stack->pointer[i]);
  }
}
#endif
