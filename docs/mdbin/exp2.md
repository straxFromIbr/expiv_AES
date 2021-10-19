
# 実験II
## 作成したプログラム
### ルックアップテーブルの出力
ルックアップテーブル`sbox.c`,`mbox02.c`,`mbox03.c`を生成するプログラムとして、
`gen_sbox.c`,`gen_mbox02.c`,`gen_mbox03.c`を作成した。

以下が作成したプログラムの内容である。
- `gen_sbox.c`

```cc
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
```

- `gen_mbox02.c`

```cc
#include "aes128.h"
#include <stdio.h>

int main(void) {
  printf("unsigned char mbox02[256] = {");
  for (unsigned char b = 0; b <= 254; b++) {
    printf("0x%02X, ", Multiply(b, 0x02));
  }
  printf("0x%02X };\n", Multiply(255, 0x02));
  return 0;
}
```

- `gen_mbox03.c`

```cc
#include "aes128.h"
#include <stdio.h>

int main(void) {
  printf("unsigned char mbox03[256] = {");
  for (unsigned char b = 0; b <= 254; b++) {
    printf("0x%02X, ", Multiply(b, 0x03));
  }
  printf("0x%02X };\n", Multiply(255, 0x03));
  return 0;
}
```

以上のプログラムを用い、次に示すMakefileでルックアップテーブルを生成した。

```Makefile
SHELL = /bin/bash
CC = /bin/cc
BIN = ./bin

# 実験2: 配列コード生成
gen_boxes: gen_sbox gen_mbox02 gen_mbox03
	@$(BIN)/gen_sbox > sbox.c
	@$(BIN)/gen_mbox02 > mbox02.c
	@$(BIN)/gen_mbox03 > mbox03.c
	@$(CC) -c sbox.c
	@$(CC) -c mbox02.c
	@$(CC) -c mbox03.c

gen_sbox: gen_sbox.o aes.o
	$(CC) -o $(BIN)/gen_sbox \
		gen_sbox.o aes.o

gen_mbox02: gen_mbox02.o aes.o
	$(CC) -o $(BIN)/gen_mbox02 \
 		gen_mbox02.o aes.o

gen_mbox03: gen_mbox03.o aes.o
	$(CC) -o $(BIN)/gen_mbox03 \
 		gen_mbox03.o aes.o

aes.o:	aes.c
	$(CC) -c aes.c
```

以上のプログラムをそれぞれコンパイルし実行ファイル

### 高速化した`SubBytes`関数,`MixColumns`関数
上記に示したルックアップテーブルを用い、高速化を図った
`SubBytes`関数と`MixColumns`関数をそれぞれ
`subbytes_lut.c`,`mixcolumns_lut.c`に作成した。


## 事前計算で得られた`sbox`の値：

## 事前計算で得られた`mbox02,mbox03`の値：

## 高速化率
