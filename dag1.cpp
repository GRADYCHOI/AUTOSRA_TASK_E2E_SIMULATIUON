#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main (void)
{
  int i, j, k,nodes = 0;
  srand (time (NULL));

  int Max = rand() % 6 + 10;
  printf ("%d = num of runnable, digraph {\n", Max);
  int input;
  printf("input num(narrow-2, wide-3) : ");
  scanf("%d", &input);
  for (i = 1; i < Max; i++) {
    for (int j = input+1; j <= Max; j++) {
      if (i <= input) {
        if ((rand() % 100) < 40) {
          printf("input");
          printf("  %d -> %d;  \n", i, j);
        }
      }
      else {
        if ((rand() % 100) < 30) {
          printf("output");
          printf("   %d -> %d;  \n", i, j);
          //if j가 0이거나 or vector 첫번째면 만들고,
          //else vector 있으면 push_back하고.
        }
      }
    }
  }

  return 0;
}