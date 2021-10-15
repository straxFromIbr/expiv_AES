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

  double b2, b4, b8, b16, b32, b64, b128, b254;

  b2 = Multiply(b, b);
  b4 = Multiply(b2, b2);
  b8 = Multiply(b4, b4);
  b16 = Multiply(b8, b8);
  b32 = Multiply(b16, b16);
  b64 = Multiply(b32, b32);
  b128 = Multiply(b64, b64);
  b254 = Multiply(
      Multiply(Multiply(Multiply(Multiply(Multiply(b2, b4), b8), b16), b32),
               b64),
      b128);
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
