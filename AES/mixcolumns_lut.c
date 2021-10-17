#include "aes128.h"
#include <stdio.h>

extern unsigned char mbox02[256];
extern unsigned char mbox03[256];

void MixColumns(unsigned char state[4 * Nb]) {

  unsigned char res[4 * Nb];
  for (int i = 0; i <= 12; i += 4) {
    res[i] =
        mbox02[state[i]] ^ mbox03[state[i + 1]] ^ state[i + 2] ^ state[i + 3];
    res[i + 1] =
        state[i] ^ mbox02[state[i + 1]] ^ mbox03[state[i + 2]] ^ state[i + 3];
    res[i + 2] =
        state[i] ^ state[i + 1] ^ mbox02[state[i + 2]] ^ mbox03[state[i + 3]];
    res[i + 3] =
        mbox03[state[i]] ^ state[i + 1] ^ state[i + 2] ^ mbox02[state[i + 3]];

    state[i] = res[i];
    state[i+1] = res[i+1];
    state[i+2] = res[i+2];
    state[i+3] = res[i+3];
  }
}
