#include "../lib/tlpi_hdr.h"
#include <signal.h>
#include <stdio.h>

static void sigHanlder(int sig) { printf("!Ouch\n"); }

int main() {
  int j;

  if (signal(SIGINT, sigHanlder) == SIG_ERR)
    errExit("signal");

  for (j = 0;; j++) {
    printf("%d\n", j);
    sleep(3);
  }
}
