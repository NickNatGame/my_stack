#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int choice = 0;
  int num = 0;
  my_stack stack;
  stack_initialize(&stack);

  while (choice != 3)
  {
    printf("Choose:\n push: [1]\n pop: [2]\n exit: [3]\n");

    if (scanf("%d", &choice) != 1)
    {
      printf("Incorrect input\n");
      int c = 0;
      while ((c = getchar()) != '\n' && c != EOF)
      {
      }
      continue;
    }

    if (choice == 1)
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
    else if (choice == 2)
    {
      pop(&stack);
    }
    else if (choice == 3)
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
