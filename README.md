# expiv_AES
情報工学実験:暗号化プログラミング

# ディレクトリ構成
- `AES/` : `*.c`,`*.o`,`Makefile`や結果などのファイル
- `docs/` : レポート
    - `Makefile` : PDF,HTMLへのエクスポート
    - `mdsrc/` : レポート原稿
        - `img/` : 画像

# イメージのビルドとコンテナの起動

```
docker-compose build
docker-compose run aesdev 
```


# 実験1
- `make test_exp1_all`を実行．正しくない関数があればエラーが出る．

# 実験2
- `make test_exp2_all`を実行．エラーがなければ`make exp2_time`を実行．実行にかかった時間が表示される．

