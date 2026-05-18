#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

const unsigned int CANARY = 0xDEADBEEF;
static const int mult = 2;
static const uintptr_t poisoned = 0xDEADBEEF;
static FILE *LOG_FILE = NULL;
static void write_canaries(my_stack *stack);
static void close_log_file(void);

static FILE *get_log_file(void)
{
  const char *log_file_name = (const char *)LOG_FILE_NAME;

  if (LOG_FILE != NULL)
  {
    return LOG_FILE;
  }

  if (log_file_name != NULL)
  {
    LOG_FILE = fopen(log_file_name, "w");
    if (LOG_FILE != NULL)
    {
      return LOG_FILE;
    }

    fprintf(stderr, "Failed to open log file '%s', using stderr\n",
            log_file_name);
  }

  LOG_FILE = stderr;

  return LOG_FILE;
}

static void close_log_file(void)
{
  if (LOG_FILE != NULL && LOG_FILE != stderr)
  {
    fclose(LOG_FILE);
  }

  LOG_FILE = NULL;
}

void stack_destroy(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  if (stack == NULL)
  {
    return;
  }

  free(stack->block);
  stack->block = (void *)poisoned;
  stack->pointer = (int *)poisoned;

  close_log_file();
}

static void
write_canaries(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  if (stack == NULL)
  {
    return;
  }

  SOFT_ASSERT_STACK(stack->block != NULL, STACK_NULL_PTR, stack);
  if (stack->block == NULL)
  {
    return;
  }

  memcpy((char *)stack->block, &CANARY, sizeof(CANARY));

  memcpy((char *)stack->block + sizeof(CANARY) + stack->capacity * sizeof(int),
         &CANARY, sizeof(CANARY));
}

int check_canaries(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  if (stack == NULL)
  {
    return 0;
  }

  SOFT_ASSERT_STACK(stack->block != NULL, STACK_NULL_PTR, stack);
  if (stack->block == NULL)
  {
    return 0;
  }

  const unsigned int *left_can_ptr = (const unsigned int *)stack->block;
  const unsigned int *right_can_ptr =
      (const unsigned int *)((const char *)stack->block +
                             sizeof(CANARY) + stack->capacity * sizeof(int));

  return (*left_can_ptr == CANARY && *right_can_ptr == CANARY);
}

void stack_initialize(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  if (stack == NULL)
  {
    return;
  }

  int total_bytes = 0;
  void *n_block = NULL;
  stack->block = NULL;
  stack->pointer = NULL;
  stack->capacity = 1;
  stack->error = STACK_OK;
  stack->size = 0;

  total_bytes = sizeof(CANARY) + stack->capacity * sizeof(int) + sizeof(CANARY);
  n_block = calloc(1, total_bytes);
  SOFT_ASSERT_STACK(n_block != NULL, STACK_MEMORY_ERR, stack);
  if (n_block == NULL)
  {
    return;
  }

  stack->block = n_block;
  stack->pointer = (int *)((char *)n_block + sizeof(CANARY));

  write_canaries(stack);
  stack->hash = hash_create(stack);
}

void stack_resize(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  if (stack == NULL)
  {
    return;
  }

  SOFT_ASSERT_STACK(stack->block != NULL, STACK_NULL_PTR, stack);
  if (stack->block == NULL)
  {
    return;
  }

  int new_capacity = 0;
  int new_total_bytes = 0;
  void *new_n_block = NULL;

  new_capacity = stack->capacity;
  if (stack->size >= stack->capacity)
  {
    new_capacity *= mult;
  }
  else if (stack->size * mult * mult <
               stack->capacity &&
           stack->capacity > 1)
  {
    new_capacity /= mult;
    new_capacity = new_capacity < 1 ? 1 : new_capacity;
  }
  else
  {
    return;
  }

  new_total_bytes =
      sizeof(CANARY) + new_capacity * sizeof(int) + sizeof(CANARY);

  new_n_block = realloc(stack->block, new_total_bytes);
  SOFT_ASSERT_STACK(new_n_block != NULL, STACK_MEMORY_ERR, stack);
  if (new_n_block == NULL)
  {
    return;
  }

  stack->block = new_n_block;
  stack->capacity = new_capacity;
  stack->pointer = (int *)((char *)stack->block + sizeof(CANARY));

  write_canaries(stack);
}

void push(my_stack *stack, int num)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  if (stack == NULL)
  {
    return;
  }

  SOFT_ASSERT_STACK(stack->block != NULL, STACK_NULL_PTR, stack);
  if (stack->block == NULL)
  {
    return;
  }

  if (stack->size >= stack->capacity)
  {
    stack_resize(stack);

    if (stack->size >= stack->capacity)
    {
      return;
    }
  }
  stack->pointer[stack->size] = num;
  stack->size += 1;

  stack->hash = hash_create(stack);
  write_canaries(stack);
}

void pop(my_stack *stack)
{
  SOFT_ASSERT_STACK(stack != NULL, STACK_NULL_PTR, stack);
  if (stack == NULL)
  {
    return;
  }

  SOFT_ASSERT_STACK(stack->block != NULL, STACK_NULL_PTR, stack);
  if (stack->block == NULL)
  {
    return;
  }

  SOFT_ASSERT_STACK(stack->size > 0, STACK_UNDERFLOW, stack);
  if (stack->size <= 0)
  {
    return;
  }

  if (stack->size > 0)
  {
    stack->size -= 1;
  }

  if (stack->size * mult * mult < stack->capacity &&
      stack->capacity > 1)
  {
    stack_resize(stack);
  }

  stack->hash = hash_create(stack);
  write_canaries(stack);
}

unsigned long hash_create(my_stack *stack)
{
  if (stack == NULL || stack->pointer == NULL)
  {
    return 0;
  }

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

  if (stack == NULL)
  {
    fprintf(out, "%s", "Null pointer at stack\n");
    fflush(out);
    return;
  }

  if (stack->error == STACK_OK)
  {
    fprintf(out, "%s", "No errors occurred\n");
    fflush(out);
    return;
  }

#define PRINT_STACK_ERROR(VALUE, STR)                                           \
  if (stack->error & (VALUE))                                                   \
  {                                                                             \
    fprintf(out, "%s", STR);                                                    \
  }

  fprintf(out, "All stack errors:\n");
#define DEF_ERROR(CODE, VALUE, STR) PRINT_STACK_ERROR(VALUE, STR)
#include "error.def"
#undef DEF_ERROR
#undef PRINT_STACK_ERROR
  fflush(out);
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

#elif LOG_LEVEL_MEDIUM
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
