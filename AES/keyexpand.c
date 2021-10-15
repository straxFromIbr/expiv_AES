#include "aes128.h"

#define subbyte(x) x = Affine(Inverse(x))

unsigned long int rcon(int n)
{
  key_t x;
  x.byte[1] = 0;
  x.byte[2] = 0;
  x.byte[3] = 0;
  if (n < 9)
  {
    x.byte[0] = 1 << (n - 1);
  }
  else
  {
    x.byte[0] = 0x1b << (n - 9);
  }
  return x.word;
}

void KeyExpansion(unsigned char key[Nk], unsigned long int w[Nb * (Nr + 1)])
{
  key_t x;
  int i;
  for (i = 0; i < Nk; i++)
  {
    int j;
    for (j = 0; j < 4; j++)
    {
      x.byte[j] = key[i * 4 + j];
    }
    w[i] = x.word;
  }
  for (i = Nk; i < Nb * (Nr + 1); i++)
  {
    unsigned long int tmp;
    tmp = w[i - 1];
    if (i % Nk == 0)
    {
      key_t x;
      key_t new;
      x.word = tmp;
      new.byte[0] = subbyte(x.byte[1]);
      new.byte[1] = subbyte(x.byte[2]);
      new.byte[2] = subbyte(x.byte[3]);
      new.byte[3] = subbyte(x.byte[0]);
      tmp = new.word;
      tmp ^= rcon(i / Nk);
    }
    else if ((Nk > 6) && (i % Nk == 4))
    {
      key_t x;
      key_t new;
      int j;
      x.word = tmp;
      for (j = 0; j < 4; j++)
      {
        new.byte[j] = subbyte(x.byte[j]);
      }
      tmp = new.word;
    }
    w[i] = w[i - Nk] ^ tmp;
  }
}
