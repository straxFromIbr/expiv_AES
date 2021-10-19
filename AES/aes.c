#include "aes128.h"

unsigned char Multiply(unsigned char x, unsigned char y) {

  unsigned char z = 0;

  while (y != 0) {
    if (y % 2 == 1) {
      z ^= x;
    }
    y = y >> 1;

    if (x >= 0x80) {
      x <<= 1;
      x ^= 0x1B;
    } else {
      x <<= 1;
    }
  }
  return z;
}

unsigned char Inverse(unsigned char b) {
  // Itoh–Tsujii inversion algorithm
  double b2, b3, b6, b12, b14, b15, b30, b60, b120, b240, b254;

  b2 = Multiply(b, b);
  b3 = Multiply(b, b2);
  b6 = Multiply(b3, b3);
  b12 = Multiply(b6, b6);
  b14 = Multiply(b2, b12);
  b15 = Multiply(b3, b12);
  b30 = Multiply(b15, b15);
  b60 = Multiply(b30, b30);
  b120 = Multiply(b60, b60);
  b240 = Multiply(b120, b120);
  b254 = Multiply(b14, b240);

  return b254;
}

unsigned char Affine(unsigned char b) {

  unsigned char result_matrix[8];
  unsigned char result_bytes = 0, b4, b2, b1;

  result_matrix[0] = 0xF1 & b;
  result_matrix[1] = 0xE3 & b;
  result_matrix[2] = 0xC7 & b;
  result_matrix[3] = 0x8F & b;

  result_matrix[4] = 0x1F & b;
  result_matrix[5] = 0x3E & b;
  result_matrix[6] = 0x7C & b;
  result_matrix[7] = 0xF8 & b;

  for (int idx = 7; idx >= 0; idx--) {
    b4 = (result_matrix[idx] >> 4) ^ result_matrix[idx];
    b2 = (b4 >> 2) ^ b4;
    b1 = (b2 >> 1 ^ b2) & 0x01;
    result_bytes = b1 ^ (result_bytes << 1);
  }

  return result_bytes ^ 0x63;
}
