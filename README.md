# expiv_AES
情報工学実験:暗号化プログラミング

## 実行
`./AES`フォルダに作成したファイルをコピー。`docker-compose run aesdev`を実行し、以下のコマンドはdockerコンテナで実行する。
### 実験1
- `make test_exp1_all`を実行。正しくない関数があればエラーが出る。

### 実験2
- `make test_exp2_all`を実行。エラーがなければ`make exp2_time`を実行。実行にかかった時間が表示される。

---
## メモ
- ガロア体(Galois Field, 有限体)：
  - 要素が有限で四則演算が閉じている集合


参考：
- [Wiki](https://ja.wikipedia.org/wiki/%E6%9C%89%E9%99%90%E4%BD%93)
- [ファイアー和田](https://ie.u-ryukyu.ac.jp/~wada/vhdl/GaloisField.html)
