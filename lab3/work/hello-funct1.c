#include "hello.h"
void helloprint1() {
  int cnt;
  printf("Hello World from funct1!\n");
  for (cnt = 0; cnt < 0x8fffff; ++cnt);
}
