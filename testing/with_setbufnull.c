#include <stdio.h>
#include <unistd.h>

int main(void) {
  setbuf(stdout, NULL);
  printf("Hello");

  sleep(5);

  printf(" World\n");
}
