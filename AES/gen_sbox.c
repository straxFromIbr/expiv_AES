#include "aes128.h"
#include <stdio.h>

int main(void) {
  unsigned char temp, res;
  printf("unsigned char sbox[256] = {");

  for (int b = 0; b <= 254; b++) {
    temp = Inverse(b);
    res = Affine(temp);
    printf("0x%02X, ", res);
  }

  printf("0x%02X };\n", Affine(Inverse(255)));
  return 0;
}
