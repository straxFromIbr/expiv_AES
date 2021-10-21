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