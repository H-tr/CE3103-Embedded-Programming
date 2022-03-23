#include "hello.h"

int main () {
  int cnt;
  printf("Hello World from main!\n");
  for (cnt = 0; cnt < 0x6fffff; ++cnt);
  helloprint1();
  helloprint2();
  printf("Bye!\n");
  return 0;
}
