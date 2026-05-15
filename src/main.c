#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

enum stack_command {
  STACK_COMMAND_PUSH = 1,
  STACK_COMMAND_POP = 2,
  STACK_COMMAND_EXIT = 3
};

int main(void)
{
  int choice = 0;
  int num = 0;
  my_stack stack;
  stack_initialize(&stack);

  while (choice != STACK_COMMAND_EXIT)
  {
    printf("Choose:\n push: [%d]\n pop: [%d]\n exit: [%d]\n",
           STACK_COMMAND_PUSH, STACK_COMMAND_POP, STACK_COMMAND_EXIT);

    if (scanf("%d", &choice) != 1)
    {
      printf("Incorrect input\n");
      int c = 0;
      while ((c = getchar()) != '\n' && c != EOF)
      {
      }
      continue;
    }

    if (choice == STACK_COMMAND_PUSH)
    {
      printf("Write number: ");
      if (scanf("%d", &num) != 1)
      {
        printf("Incorrect number input\n");
        int c = 0;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        continue;
      }
      push(&stack, num);
    }
    else if (choice == STACK_COMMAND_POP)
    {
      pop(&stack);
    }
    else if (choice == STACK_COMMAND_EXIT)
    {
      break;
    }
    else
    {
      printf("Unknown command\n");
    }

    dump(&stack);
  }

  stack_destroy(&stack);
  return 0;
}
