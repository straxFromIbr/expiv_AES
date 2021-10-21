# 4.2 Multiplication
多項式表現において、GF(2^8)内での積演算は、多項式の積と8次既約多項式mのあまりで表される．AESでは(01)H(1b)Hで表される既約多項式を用いる．

多項式表現において、2^8のガロア体における積は、多項式の積の既約8次多項式を法とした余剰を求めることに対応する．AES暗号においてその法は m(x) = x^8+x^4+x+1で表される．




---
## メモ
- ガロア体(Galois Field, 有限体)：
  - 要素が有限で四則演算が閉じている集合


参考：
- [Wiki](https://ja.wikipedia.org/wiki/%E6%9C%89%E9%99%90%E4%BD%93)
- [ファイアー和田](https://ie.u-ryukyu.ac.jp/~wada/vhdl/GaloisField.html)
