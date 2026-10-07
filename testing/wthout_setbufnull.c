#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  printf("Hello");
  int a;
  scanf("%d", &a);

  sleep(5);

  printf(" World\n");
}
