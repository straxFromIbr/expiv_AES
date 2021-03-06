
## 1. 目的
- 広く利用されているAES暗号の暗号化プログラムを実装することで、対称ブロック暗号(symmetric block cipher)について学ぶ．
- 平文と暗号文の線形性を壊すために、離散数学(ガロア体理論に基づくバイト演算)が応用されていることを学ぶ．
- 暗号化で不可欠なビット演算のプログラミングを学ぶ．
- 事前計算(precalculation)とルックアップテーブル(lookup table)を利用した高速化を学ぶ．


---


### 実行環境
Dockerを用いて実験の環境を構築した．比較のためにネイティブで動作するUbuntuでも動作の確認をした．
1. OS: macOS 12.0.1 21A558 arm64
    - Docker: 20.10.8, build 3967b7d
        - カーネル: 5.10.47-linuxkit
        - bash: 5.0.17
        - cc: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
        - make: GNU Make 4.2.1

2. OS: Ubuntu 20.04.2 LTS x86_6
    - カーネル: 5.8.0-63-generic
    - bash: 5.0.17
    - cc: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
    - make: GNU Make 4.2.1

コンテナの作成には次のファイルを用い、フラグとして`--platform='linux/amd64'`を指定しビルドした．

```Dockerfile
FROM ubuntu:latest
WORKDIR /home
RUN apt update && \
    apt upgrade -y && \
    apt install gcc make -y
CMD ["/usr/bin/bash","-l"]
```

また実験に使用したソースコードやMakefileなどの全体は
Gihubリポジトリ[expiv_AES](https://github.com/straxFromIbr/expiv_AES/tree/4261c1885b9cceecd881efcf41addbdffab238e7)に公開している．

---


## 2. 実験I

### 2.1. 作成したプログラム
以下に示すのは`Multiply`関数、`Inverse`関数、`Affine`関数を含む`aes.c`のプログラム全体である．

```c {.line-numbers}
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
```

### 2.2. 利用した`Inverse`のアルゴリズム
伊藤辻井アルゴリズムと呼ばれる方式を使用した．逆元を求めたいバイトデータをbとするとき、このアルゴリズムは次の式で表される．なお`*`はMultiply関数による積を示す．

```
  b2 =    b *    b  
  b3 =    b *   b2  
  b6 =   b3 *   b3  
 b12 =   b6 *   b6  
 b14 =   b2 *  b12  
 b15 =   b3 *  b12  
 b30 =  b15 *  b15  
 b60 =  b30 *  b30  
b120 =  b60 *  b60  
b240 = b120 * b120  
b254 =  b14 * b240  
```

このアルゴリズムにより一度のInverseの処理を11回の積で実現できる．


### 2.3. `Affine`関数のフローチャート
![Affine関数フローチャート](mdsrc/img/affine_fc.png)

### 2.4. `Multiply`アルゴリズムの正当性
<!-- TODO -->

AESのアルゴリズムにおいて8ビットデータは、各桁の値を係数とした7次多項式で表現される．
例えば $(10010011)_2 \rightarrow x^7+x^4+x^1+1$ のような具合である．

さらに2つの8バイトデータ $a$ , $b$ の和は各桁ごとの排他的論理和、積は $a$, $b$ それぞれの多項式表現の積の、
既約な8次多項式 $m(x)$ を法とする余剰類であり、AESにおいては $m(x) = x^8 + x^4 + x^3 + x + 1$ を利用している．

次に、 8ビットデータ$a=(a_7 a_6 a_5 a_4 a_3 a_2 a_1 a_0)_2$ と$x$(すなわち2進数表現での$(00000010)_2$)の積を考える．

第1に、$a_7$ が0であるとき、 各項の係数を$a_i$とすると
$$\begin{aligned}
a\cdot x  &= (a_6 \cdot x^6 + a_5 \cdot x^5 + a_4 \cdot x^4 + a_3 \cdot x^3 + a_2 \cdot x^2 + a_1 \cdot x^1 + a_0 \cdot 1) \cdot x \mod m(x)\\
          &= (a_6 \cdot x^7 + a_5 \cdot x^6 + a_4 \cdot x^5 + a_3 \cdot x^4 + a_2 \cdot x^3 + a_1 \cdot x^2 + a_0 \cdot x) \mod m(x) \\
          &= (a_6 \cdot x^7 + a_5 \cdot x^6 + a_4 \cdot x^5 + a_3 \cdot x^4 + a_2 \cdot x^3 + a_1 \cdot x^2 + a_0 \cdot x) \\
          &= (a_6a_5a_4a_3a_2a_1a_00)_2
\end{aligned}$$

となる．
これは $a$ の2進数表現を1ビット左にシフトしたことに対応する．

第2に、$a_7$ が1であるとき、 

$$\begin{aligned}
a\cdot x  &= (x^7 + a_6 \cdot x^6 + a_5 \cdot x^5 + a_4 \cdot x^4 + a_3 \cdot x^3 + a_2 \cdot x^2 + a_1 \cdot x^1 + a_0 \cdot 1) \cdot x \mod m(x) \\
          &= (x^8 + a_6 \cdot x^7 + a_5 \cdot x^6 + a_4 \cdot x^5 + a_3 \cdot x^4 + a_2 \cdot x^3 + a_1 \cdot x^2 + a_0 \cdot x^1 ) \mod m(x) \\
          &= x^8 \mod m(x) + (a_6 \cdot x^7 + a_5 \cdot x^6 + a_4 \cdot x^5 + a_3 \cdot x^4 + a_2 \cdot x^3 + a_1 \cdot x^2 + a_0 \cdot x^1 )  \\
          &= x^8 \mod (x^8 + x^4 + x^3 + x + 1) + (a_6 \cdot x^7 + a_5 \cdot x^6 + a_4 \cdot x^5 + a_3 \cdot x^4 + a_2 \cdot x^3 + a_1 \cdot x^2 + a_0 \cdot x^1 )  \\
          &= (x^4 + x^3 + x^1 + 1) + (a_6 \cdot x^7 + a_5 \cdot x^6 + a_4 \cdot x^5 + a_3 \cdot x^4 + a_2 \cdot x^3 + a_1 \cdot x^2 + a_0 \cdot x^1 )  \\
          &= (00011011)_2 \oplus (a_6a_5a_4a_3a_2a_1a_00)_2
\end{aligned}$$
となり、これは $a$ 2進数表現を1ビット左にシフトし、 $(81)_H$ とのXORをとった値に対応する．

これらの演算を次に示す $xtime$ の操作として定義する．
$$\begin{aligned}
xtime(a)=
\begin{cases}
\text{LSHIFT $a$}  & \text{if $a_7=0$,}\\
\text{$(81)_H\oplus$ LSHIFT $a$  } & \text{else.}
\end{cases}
\end{aligned}$$

以上を踏まえ、2つの8ビットバイトデータ   
$a=(a_7 a_6 a_5 a_4 a_3 a_2 a_1 a_0)_2$ と  
$b=(b_7 b_6 b_5 b_4 b_3 b_2 b_1 b_0)_2$ の積をとる時、  

$$ \begin{aligned}
  a\cdot b &= a\cdot((b_70000000)_2 \oplus (0b_6000000)_2 \oplus (00b_500000)_2 \oplus ... \oplus (0000000{b_0})_2) \\
  &= a\cdot(b_70000000)_2 \oplus a\cdot(0b_6000000)_2 \oplus a\cdot(00b_500000)_2 \oplus ... \oplus a\cdot(0000000b_0)_2 
 \end{aligned} $$

と変形し $xtime$ を適用することで $a$ $b$ の積を求められるようにする．

$b_i$ の値が1のとき、上式で $b_i$ を含む項の値は $a$ に $xtime$ を $i$ 回適用した値に等しい．
なぜなら $b_i$ のみが1であるような8ビットデータの多項式表現は $x^i$ のためである．

例えば $i=3$ のとき、$a\cdot(00001000)_2$ は $xtime(xtime(xtime(a)))$ と計算できる．

$xtime$ を $n$ 回適用する操作を $xtime^n$ とするとき、 $a\cdot b$ は

$$ \begin{aligned}
\sum_{i=0}^7{xtime^i(a)} \tag{1}
\end{aligned} $$

(ただし和は排他的論理和)とできる．

`Multiply`関数の擬似コードをみると、

1. $z=0$
2. $b_0$ が1であれば、 $z = z \oplus a$
3. $a=xtime(a)$
4. $b$ を1ビット左にシフト
5. $b$ が0でないならば`2`に戻る

となっている．手順2,3に注目すると、 $b_i$ が1であるとき、 $z$ の値は $xtime^i(a)$ 
であることがわかり、この`Multiply`の擬似コードは前述した2つのバイトデータ, $a$ , $b$ の積 $z$ を返すことが確認できる．



### 2.5. 4バイトデータと1ワードデータの双方向変換の`union`による実現
keyexpand.cでは、4バイトデータと1ワードデータの相互変換をaes128.hで定義される`key_t`型のunion(共用体)変数を用いて行っている．

C言語においてunionとは、同じメモリ領域を複数の型の変数が共有する共用体のデータ構造である．

`key_t`型では`unsigned long int`型の`word`(32ビット)と`unsigned char[4]`型の`byte`(8ビット*4)が同じメモリ領域を共有している．
unionの初期化は先頭のメンバ変数の型で行われるため、`key_t`型の場合、メモリ上に32ビットの連続した領域が確保される．

例えば`0x12345678`というデータで`key_t`型の変数`x`を初期化すると、`x.word`でワードデータとして、
`x.byte[1]`で2バイト目のデータ(0x34)を得ることができる．


### 2.6. `test1.o`で用いたブロックと暗号鍵、及び実行結果

次のようにして`aes.c`をコンパイルし実行ファイル`bin/exp1`を作成した．

```
/bin/cc -o bin/exp1 \
  aes.c \
  cipherH.o debug.o shiftrows.o addroundkey.o \
  subbytes.o mixcolumns.o \
  keyexpand1.o test1.o
```

- 使用したブロック(PLAINTEXT)
  - `00112233445566778899aabbccddeeff`
- 使用した暗号鍵(KEY)
  - `000102030405060708090a0b0c0d0e0f`
- `bin/exp1`の実行結果

```
PLAINTEXT:       00112233445566778899aabbccddeeff
KEY:             000102030405060708090a0b0c0d0e0f
CIPHER (ENCRIPT):
round[ 0].input  00112233445566778899aabbccddeeff
round[ 0].k_sch  000102030405060708090a0b0c0d0e0f
round[ 1].start  00102030405060708090a0b0c0d0e0f0
round[ 1].s_box  63cab7040953d051cd60e0e7ba70e18c
round[ 1].s_row  6353e08c0960e104cd70b751bacad0e7
round[ 1].m_col  5f72641557f5bc92f7be3b291db9f91a
round[ 1].k_sch  d6aa74fdd2af72fadaa678f1d6ab76fe
round[ 2].start  89d810e8855ace682d1843d8cb128fe4
round[ 2].s_box  a761ca9b97be8b45d8ad1a611fc97369
round[ 2].s_row  a7be1a6997ad739bd8c9ca451f618b61
round[ 2].m_col  ff87968431d86a51645151fa773ad009
round[ 2].k_sch  b692cf0b643dbdf1be9bc5006830b3fe
round[ 3].start  4915598f55e5d7a0daca94fa1f0a63f7
round[ 3].s_box  3b59cb73fcd90ee05774222dc067fb68
round[ 3].s_row  3bd92268fc74fb735767cbe0c0590e2d
round[ 3].m_col  4c9c1e66f771f0762c3f868e534df256
round[ 3].k_sch  b6ff744ed2c2c9bf6c590cbf0469bf41
round[ 4].start  fa636a2825b339c940668a3157244d17
round[ 4].s_box  2dfb02343f6d12dd09337ec75b36e3f0
round[ 4].s_row  2d6d7ef03f33e334093602dd5bfb12c7
round[ 4].m_col  6385b79ffc538df997be478e7547d691
round[ 4].k_sch  47f7f7bc95353e03f96c32bcfd058dfd
round[ 5].start  247240236966b3fa6ed2753288425b6c
round[ 5].s_box  36400926f9336d2d9fb59d23c42c3950
round[ 5].s_row  36339d50f9b539269f2c092dc4406d23
round[ 5].m_col  f4bcd45432e554d075f1d6c51dd03b3c
round[ 5].k_sch  3caaa3e8a99f9deb50f3af57adf622aa
round[ 6].start  c81677bc9b7ac93b25027992b0261996
round[ 6].s_box  e847f56514dadde23f77b64fe7f7d490
round[ 6].s_row  e8dab6901477d4653ff7f5e2e747dd4f
round[ 6].m_col  9816ee7400f87f556b2c049c8e5ad036
round[ 6].k_sch  5e390f7df7a69296a7553dc10aa31f6b
round[ 7].start  c62fe109f75eedc3cc79395d84f9cf5d
round[ 7].s_box  b415f8016858552e4bb6124c5f998a4c
round[ 7].s_row  b458124c68b68a014b99f82e5f15554c
round[ 7].m_col  c57e1c159a9bd286f05f4be098c63439
round[ 7].k_sch  14f9701ae35fe28c440adf4d4ea9c026
round[ 8].start  d1876c0f79c4300ab45594add66ff41f
round[ 8].s_box  3e175076b61c04678dfc2295f6a8bfc0
round[ 8].s_row  3e1c22c0b6fcbf768da85067f6170495
round[ 8].m_col  baa03de7a1f9b56ed5512cba5f414d23
round[ 8].k_sch  47438735a41c65b9e016baf4aebf7ad2
round[ 9].start  fde3bad205e5d0d73547964ef1fe37f1
round[ 9].s_box  5411f4b56bd9700e96a0902fa1bb9aa1
round[ 9].s_row  54d990a16ba09ab596bbf40ea111702f
round[ 9].m_col  e9f74eec023020f61bf2ccf2353c21c7
round[ 9].k_sch  549932d1f08557681093ed9cbe2c974e
round[10].start  bd6e7c3df2b5779e0b61216e8b10b689
round[10].s_box  7a9f102789d5f50b2beffd9f3dca4ea7
round[10].s_row  7ad5fda789ef4e272bca100b3d9ff59f
round[10].k_sch  13111d7fe3944a17f307a78b4d2b30c5
round[10].output 69c4e0d86a7b0430d8cdb78070b4c55a
```

---



## 3. 実験II
### 3.1. 作成したプログラム
#### 3.1.1. ルックアップテーブルの出力
ルックアップテーブル`sbox.c`,`mbox02.c`,`mbox03.c`を生成するプログラムとして、
`gen_sbox.c`,`gen_mbox02.c`,`gen_mbox03.c`を作成した．

以下がルックアップテーブルの出力プログラムである．
- `gen_sbox.c`

```c
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

```c
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

```c
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

以上のプログラムを用い、次に示すMakefileでルックアップテーブルを生成した．

```Makefile
SHELL = /bin/bash
CC = /bin/cc -O3
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

#### 3.1.2. 高速化した`SubBytes`関数,`MixColumns`関数
上記に示したルックアップテーブルを用い、高速化を図った
`SubBytes`関数と`MixColumns`関数をそれぞれ
`subbytes_lut.c`,`mixcolumns_lut.c`に作成した．

- `subbytes_lut.c`

```c
#include "aes128.h"

extern unsigned char sbox[256];

void SubBytes(unsigned char state[4 * Nb]) {
  for (int i = 0; i < 4 * Nb; i++) {
    state[i] = sbox[state[i]];
  }
}
```

- `mixcolumns_lut.c`

```c
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
    state[i + 1] = res[i + 1];
    state[i + 2] = res[i + 2];
    state[i + 3] = res[i + 3];
  }
}
```
### 3.2. 事前計算で得られたルックアップテーブルの値
#### 3.2.1. `sbox`:
`make gen_boxes`を実行し得られた`sbox.c`を見やすさのために改行を挿入した．

- `sbox.c`

```c
unsigned char sbox[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B,
    0xFE, 0xD7, 0xAB, 0x76, 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0, 0xB7, 0xFD, 0x93, 0x26,
    0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2,
    0xEB, 0x27, 0xB2, 0x75, 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84, 0x53, 0xD1, 0x00, 0xED,
    0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F,
    0x50, 0x3C, 0x9F, 0xA8, 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2, 0xCD, 0x0C, 0x13, 0xEC,
    0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14,
    0xDE, 0x5E, 0x0B, 0xDB, 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79, 0xE7, 0xC8, 0x37, 0x6D,
    0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F,
    0x4B, 0xBD, 0x8B, 0x8A, 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E, 0xE1, 0xF8, 0x98, 0x11,
    0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F,
    0xB0, 0x54, 0xBB, 0x16};

```

#### 3.2.2. `mbox02,mbox03`：
こちらも`sbox.c`同様に改行を挿入した

- `mbox02.c`

```c
unsigned char mbox02[256] = {
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x16,
    0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E,
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x40, 0x42, 0x44, 0x46,
    0x48, 0x4A, 0x4C, 0x4E, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5A, 0x5C, 0x5E,
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E, 0x70, 0x72, 0x74, 0x76,
    0x78, 0x7A, 0x7C, 0x7E, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E,
    0x90, 0x92, 0x94, 0x96, 0x98, 0x9A, 0x9C, 0x9E, 0xA0, 0xA2, 0xA4, 0xA6,
    0xA8, 0xAA, 0xAC, 0xAE, 0xB0, 0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE,
    0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE, 0xD0, 0xD2, 0xD4, 0xD6,
    0xD8, 0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE,
    0xF0, 0xF2, 0xF4, 0xF6, 0xF8, 0xFA, 0xFC, 0xFE, 0x1B, 0x19, 0x1F, 0x1D,
    0x13, 0x11, 0x17, 0x15, 0x0B, 0x09, 0x0F, 0x0D, 0x03, 0x01, 0x07, 0x05,
    0x3B, 0x39, 0x3F, 0x3D, 0x33, 0x31, 0x37, 0x35, 0x2B, 0x29, 0x2F, 0x2D,
    0x23, 0x21, 0x27, 0x25, 0x5B, 0x59, 0x5F, 0x5D, 0x53, 0x51, 0x57, 0x55,
    0x4B, 0x49, 0x4F, 0x4D, 0x43, 0x41, 0x47, 0x45, 0x7B, 0x79, 0x7F, 0x7D,
    0x73, 0x71, 0x77, 0x75, 0x6B, 0x69, 0x6F, 0x6D, 0x63, 0x61, 0x67, 0x65,
    0x9B, 0x99, 0x9F, 0x9D, 0x93, 0x91, 0x97, 0x95, 0x8B, 0x89, 0x8F, 0x8D,
    0x83, 0x81, 0x87, 0x85, 0xBB, 0xB9, 0xBF, 0xBD, 0xB3, 0xB1, 0xB7, 0xB5,
    0xAB, 0xA9, 0xAF, 0xAD, 0xA3, 0xA1, 0xA7, 0xA5, 0xDB, 0xD9, 0xDF, 0xDD,
    0xD3, 0xD1, 0xD7, 0xD5, 0xCB, 0xC9, 0xCF, 0xCD, 0xC3, 0xC1, 0xC7, 0xC5,
    0xFB, 0xF9, 0xFF, 0xFD, 0xF3, 0xF1, 0xF7, 0xF5, 0xEB, 0xE9, 0xEF, 0xED,
    0xE3, 0xE1, 0xE7, 0xE5};

```

- `mbox03.c`

```c
unsigned char mbox03[256] = {
    0x00, 0x03, 0x06, 0x05, 0x0C, 0x0F, 0x0A, 0x09, 0x18, 0x1B, 0x1E, 0x1D,
    0x14, 0x17, 0x12, 0x11, 0x30, 0x33, 0x36, 0x35, 0x3C, 0x3F, 0x3A, 0x39,
    0x28, 0x2B, 0x2E, 0x2D, 0x24, 0x27, 0x22, 0x21, 0x60, 0x63, 0x66, 0x65,
    0x6C, 0x6F, 0x6A, 0x69, 0x78, 0x7B, 0x7E, 0x7D, 0x74, 0x77, 0x72, 0x71,
    0x50, 0x53, 0x56, 0x55, 0x5C, 0x5F, 0x5A, 0x59, 0x48, 0x4B, 0x4E, 0x4D,
    0x44, 0x47, 0x42, 0x41, 0xC0, 0xC3, 0xC6, 0xC5, 0xCC, 0xCF, 0xCA, 0xC9,
    0xD8, 0xDB, 0xDE, 0xDD, 0xD4, 0xD7, 0xD2, 0xD1, 0xF0, 0xF3, 0xF6, 0xF5,
    0xFC, 0xFF, 0xFA, 0xF9, 0xE8, 0xEB, 0xEE, 0xED, 0xE4, 0xE7, 0xE2, 0xE1,
    0xA0, 0xA3, 0xA6, 0xA5, 0xAC, 0xAF, 0xAA, 0xA9, 0xB8, 0xBB, 0xBE, 0xBD,
    0xB4, 0xB7, 0xB2, 0xB1, 0x90, 0x93, 0x96, 0x95, 0x9C, 0x9F, 0x9A, 0x99,
    0x88, 0x8B, 0x8E, 0x8D, 0x84, 0x87, 0x82, 0x81, 0x9B, 0x98, 0x9D, 0x9E,
    0x97, 0x94, 0x91, 0x92, 0x83, 0x80, 0x85, 0x86, 0x8F, 0x8C, 0x89, 0x8A,
    0xAB, 0xA8, 0xAD, 0xAE, 0xA7, 0xA4, 0xA1, 0xA2, 0xB3, 0xB0, 0xB5, 0xB6,
    0xBF, 0xBC, 0xB9, 0xBA, 0xFB, 0xF8, 0xFD, 0xFE, 0xF7, 0xF4, 0xF1, 0xF2,
    0xE3, 0xE0, 0xE5, 0xE6, 0xEF, 0xEC, 0xE9, 0xEA, 0xCB, 0xC8, 0xCD, 0xCE,
    0xC7, 0xC4, 0xC1, 0xC2, 0xD3, 0xD0, 0xD5, 0xD6, 0xDF, 0xDC, 0xD9, 0xDA,
    0x5B, 0x58, 0x5D, 0x5E, 0x57, 0x54, 0x51, 0x52, 0x43, 0x40, 0x45, 0x46,
    0x4F, 0x4C, 0x49, 0x4A, 0x6B, 0x68, 0x6D, 0x6E, 0x67, 0x64, 0x61, 0x62,
    0x73, 0x70, 0x75, 0x76, 0x7F, 0x7C, 0x79, 0x7A, 0x3B, 0x38, 0x3D, 0x3E,
    0x37, 0x34, 0x31, 0x32, 0x23, 0x20, 0x25, 0x26, 0x2F, 0x2C, 0x29, 0x2A,
    0x0B, 0x08, 0x0D, 0x0E, 0x07, 0x04, 0x01, 0x02, 0x13, 0x10, 0x15, 0x16,
    0x1F, 0x1C, 0x19, 0x1A};

```

### 3.3. 高速化率の比較

`time`コマンドを使用して計測した`user`タイムと, 
SubBytes, MixColumnsのどちらも実験Iのものを使用した実行ファイルとの
速度の比率を次の表に示す．
なお、使用した実行ファイルはDockerでもネイティブなLinuxでも同じものを使用している．


- Docker

|                    | `MixColumns`(実験I) | `MixColumns`(実験II) |
| :----------------- | ------------------: | -------------------: |
| `SubBytes`(実験I)  |     36.642s(1.0000) |      35.349s(1.0366) |
| `SubBytes`(実験II) |     35.288s(1.0384) |      33.684s(1.0878) |

- ネイティブなLinux

|                    | `MixColumns`(実験I) | `MixColumns`(実験II) |
| :----------------- | ------------------: | -------------------: |
| `SubBytes`(実験I)  |       6.038s(1.000) |        5.811s(1.039) |
| `SubBytes`(実験II) |       4.750s(1.271) |        4.376s(1.380) |


この結果を見ると、特にネイティブなLinuxにおいてMixColumns関数の高速化よりもSubBytes関数の高速化の効果が高いことがわかる．Dockerにおいても僅かではあるがその傾向がとれる．

<!-- 
* SubBytes : InverseとAffineを16回
  * Inverse: Multiplyを11回 -> 176回
* MixColumns : Multiplyを8*16回 -> 128回
-->

---


## 4. 考察
### 4.1. 実験II:ルックアップテーブルの使用による高速化
`SubBytes`関数にテーブルを適用した場合と`MixColumns`にテーブルを適用した場合では明らかに高速化率に差があった．

Cipherサブルーチンの内部でSubBytes関数とMixColumns関数を呼び出す回数は、最終回に`SubBytes`関数が余分に1回呼ばれるだけにとどまり、それだけでは有意な差を生む要因とはならないと考えられるため、
２つの関数の内部の計算コストの違いについて検討する．特に内部で呼び出しているユーザ定義関数が、
配列変数へのアクセスやビット演算に比べ計算コストが高いと思われるため、それらの関数の呼び出し回数について調べる．

資料中のSubBytes関数の擬似コードを見ると、内部でInverse関数とAffine関数を16回ずつ呼び出していることがわかる．
さらにInverse関数は今回の実装ではMultiply関数を11回呼び出しているため、
合計でMultiply関数を176回とAffine関数を16回呼び出している．

一方MixColumns関数は主な処理としてMultiply関数を128回呼び出している．

よってSubBytes関数がMultiply関数を48回多く、Affine関数を16回多く呼び出していることが
高速化に差がある原因だと考えられる．

### 4.2. その他
実験の趣旨とは異なるが、Docker環境で実行した場合と、ネイティブに実行されるLinuxで、
実行速度に大きな差がでた．またDockerでの高速化率がネイティブLinux環境に比べ非常に小さい．
その要因として、ホストマシンとコンテナのアーキテクチャが異なるために、何らかの変換レイヤー
によるオーバーヘッドがあることが考えられる．


## 5. 感想
今回の実験では、主にAESの基礎的な理論の理解を進めることに重点が置かれていたが、
実際に暗号として運用することを考えた場合、共通鍵の保管方法など考慮するべき点が多々
あると感じた．



---


