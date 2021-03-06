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

