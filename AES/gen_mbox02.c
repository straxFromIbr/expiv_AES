#include <stdio.h>

#include "aes128.h"

int main(void) {
  printf("unsigned char mbox02[256] = {");

  for (int b = 0; b <= 254; b++) {
    printf("0x%02X, ", Multiply(b, 0x02));
  }

  printf("0x%02X };\n", Multiply(255, 0x02));
  return 0;
}
