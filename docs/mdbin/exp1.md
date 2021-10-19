# 実験I

## 作成したプログラム
以下に示すのは`Multiply`関数、`Inverse`関数、`Affine`関数を含む`aes.c`のプログラム全体である。

```cc
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
```

## 利用した`Inverse`のアルゴリズム
上記の`aes.c`に示した通り、Inverse関数のアルゴリズムとして、資料に示されてる「計算方法2」を使用した。

## `Affine`関数のフローチャート
![Affine関数フローチャート](img/affine_fc.png)

## `Multiply`アルゴリズムの正当性
## 4バイトデータと1ワードデータの双方向変換の`union`による実現
keyexpand.cでは、4バイトデータと1ワードデータの相互変換をaes128.hで定義される`key_t`型のunion(共用体)変数を用いて行っている。

C言語においてunionとは、同じメモリ領域を複数の型の変数が共有する共用体のデータ構造である。

`key_t`型では`unsigned long int`型の`word`(32ビット)と`unsigned char[4]`型の`byte`(8ビット*4)が同じメモリ領域を共有している。
unionの初期化は先頭のメンバ変数の型で行われるため、`key_t`型の場合、メモリ上に32ビットの連続した領域が確保される。

例えば`0x12345678`というデータで`key_t`型の変数`x`を初期化すると、`x.word`でワードデータとして、
`x.byte[1]`で2バイト目のデータ(0x34)を得ることができる。


