#include "aes128.h"

extern unsigned char sbox[256];

void SubBytes(unsigned char state[4 * Nb]) {
  for (int i = 0; i < 4 * Nb; i++) {
    state[i] = sbox[state[i]];
  }
}
