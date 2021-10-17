#include "aes128.h"
#include <stdio.h>

static unsigned char mbox02[256];
static unsigned char mbox03[256];

void MixColumns(unsigned char state[4 * Nb]) {
  unsigned char res[12];
  for (int i = 0; i < 12; i += 4) {
    res[i] =
        mbox02[state[i]] ^ mbox03[state[i + 1]] ^ state[i + 2] ^ state[i + 3];
    res[i + 1] =
        state[i] ^ mbox02[state[i + 1]] ^ mbox03[state[i + 2]] ^ state[i + 3];
    res[i + 2] =
        state[i] ^ state[i + 1] ^ mbox02[state[i + 2]] ^ mbox03[state[i + 3]];
    res[i + 3] =
        mbox03[state[i]] ^ state[i + 1] ^ state[i + 2] ^ mbox02[state[i + 3]];
  }
  state = res;
}
