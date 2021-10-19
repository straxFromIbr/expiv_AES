# 暗号化プログラミングレポート

# 目的
- 広く利用されているAES暗号の暗号化プログラムを実装することで、対称ブロック暗号(symmetric block cipher)について学ぶ。
- 平文と暗号文の線形性を壊すために、離散数学(ガロア体理論に基づくバイト演算)が応用されていることを学ぶ。
- 暗号化で不可欠なビット演算のプログラミングを学ぶ。
- 事前計算(precalculation)とルックアップテーブル(lookup table)を利用した高速化を学ぶ。

----
## 実行環境
Dockerを用いて実験の環境を構築した。比較のためにネイティブで動作するUbuntuでも動作の確認をした。
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

コンテナは次のファイルを用い、フラグとして`--platform='linux/amd64'`を指定しビルドした。

```Dockerfile
FROM ubuntu:latest
WORKDIR /home
RUN apt update && \
    apt upgrade -y && \
    apt install gcc make -y
CMD ["/usr/bin/bash","-l"]
```
