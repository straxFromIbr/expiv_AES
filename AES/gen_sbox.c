#include "aes128.h"
#include <stdio.h>

int main(void) {
  printf("unsigned char sbox[256] = {");

  for (int b = 0; b <= 254; b++) {
    printf("0x%02X, ", Affine(Inverse(b)));
  }

  printf("0x%02X };\n", Affine(Inverse(255)));
  return 0;
}
