SHELL=/bin/bash
CC = /usr/bin/cc
BIN = bin
basefiles = aes.o cipherH.o shiftrows.o  addroundkey.o debug.o
base_sm_files = $(basefiles) subbytes.o mixcolumns.o
TESTER = test1.o

to_test1:
	$(eval TESTER := test1.o)

to_test2:
	$(eval TESTER := test2.o)


# 実験1:関数ごとのテストとaes.cのテストを全て実行
test_exp1_all: to_test1 check_affine check_multiply check_inverse check_aes
	@echo done

# 実験2:
# 出力のチェック
test_exp2_all: to_test1  exp2_11 exp2_12 exp2_21 exp2_22
	@set pipefaile
	@echo -n "EXP2_11 : "
	@diff ./test1.res.txt <($(BIN)/exp2_11) >/dev/null && echo 'passed'|| echo 'FAILED'
	@echo -n "EXP2_12 : "
	@diff ./test1.res.txt <($(BIN)/exp2_12) >/dev/null && echo 'passed'|| echo 'FAILED'
	@echo -n "EXP2_21 : "
	@diff ./test1.res.txt <($(BIN)/exp2_21) >/dev/null && echo 'passed'|| echo 'FAILED'
	@echo -n "EXP2_22 : "
	@diff ./test1.res.txt <($(BIN)/exp2_22) >/dev/null && echo 'passed'|| echo 'FAILED'
	

# タイム計測
exp2_time: to_test2 exp2_11 exp2_12 exp2_21 exp2_22
	@echo 'EXP2_11'
	@echo '-------------'
	@time $(BIN)/exp2_11 >/dev/null
	@echo -e '\n\n'
	@echo 'EXP2_12'
	@echo '-------------'
	@time $(BIN)/exp2_12 >/dev/null
	@echo -e '\n\n'
	@echo 'EXP2_21'
	@echo '-------------'
	@time $(BIN)/exp2_21 >/dev/null
	@echo -e '\n\n'
	@echo 'EXP2_22'
	@echo '-------------'
	@time $(BIN)/exp2_22 >/dev/null



# 実験2:関数の組み合わせを変えた実行ファイルのコンパイル
exp2_11: aes.o
	$(CC) -o $(BIN)/exp2_11 \
		$(basefiles) \
		keyexpand1.o  \
		subbytes.o mixcolumns.o \
		$(TESTER)

exp2_12: aes.o gen_boxes mixcolumns_lut
	$(CC) -o $(BIN)/exp2_12 \
		$(basefiles) \
		keyexpand1.o \
		subbytes.o mixcolumns_lut.o mbox02.o mbox03.o \
		$(TESTER)

exp2_21: aes.o gen_boxes subbytes_lut
	$(CC) -o $(BIN)/exp2_21 \
		$(basefiles) \
		keyexpand2.o \
		subbytes_lut.o sbox.o mixcolumns.o \
		$(TESTER)

exp2_22: aes.o gen_boxes subbytes_lut mixcolumns_lut
	$(CC) -o $(BIN)/exp2_22 \
		$(basefiles) \
		keyexpand2.o \
		subbytes_lut.o sbox.o mixcolumns_lut.o mbox02.o mbox03.o \
		$(TESTER)

gen_boxes: gen_sbox gen_mbox02 gen_mbox03
	@$(BIN)/gen_sbox > sbox.c
	@$(BIN)/gen_mbox02 > mbox02.c
	@$(BIN)/gen_mbox03 > mbox03.c
	@$(CC) -c sbox.c
	@$(CC) -c mbox02.c
	@$(CC) -c mbox03.c

# --------実験1---------
## 実験1:aes.c全体のテスト
check_aes: to_test1 aes
	@echo -n 'AES : '
	@diff ./test1.res.txt <($(BIN)/exp1)  >/dev/null && echo 'passed'|| echo 'FAILED'

## 実験1:関数ごとのテスト
check_affine: to_test1 test_affine
	@echo -n 'AFFINE : '
	@diff ./testAffine.res.txt <($(BIN)/test_affine) >/dev/null && echo 'passed'|| echo 'FAILED'

check_multiply: to_test1 test_multiply
	@echo -n 'MULTIPLY : '
	@diff ./testMultiply.res.txt <($(BIN)/test_multiply) >/dev/null && echo 'passed'|| echo 'FAILED'


check_inverse: to_test1 test_inverse
	@echo -n 'INVERSE : '
	@diff ./testInverse.res.txt <($(BIN)/test_inverse) >/dev/null && echo 'passed'|| echo 'FAILED'


# 実験1:関数ごとのテスト実行ファイルのコンパイル
test_affine: aes
	@$(CC) -o $(BIN)/test_affine \
		$(base_sm_files) keyexpand1.o \
		testAffine.o

test_multiply: aes
	@$(CC) -o $(BIN)/test_multiply \
		$(base_sm_files) keyexpand1.o \
		testMultiply.o

test_inverse: aes
	@$(CC) -o $(BIN)/test_inverse \
		$(base_sm_files) keyexpand1.o \
		testInverse.o

# 実験1用の実行ファイル
aes: aes.o
	@$(CC) -o $(BIN)/exp1 \
		$(base_sm_files) keyexpand1.o \
		$(TESTER)

# --------実験2---------
gen_sbox: gen_sbox.o aes.o
	$(CC) -o $(BIN)/gen_sbox \
		gen_sbox.o aes.o

gen_mbox02: gen_mbox02.o aes.o
	$(CC) -o $(BIN)/gen_mbox02 \
 		gen_mbox02.o aes.o

gen_mbox03: gen_mbox03.o aes.o
	$(CC) -o $(BIN)/gen_mbox03 \
 		gen_mbox03.o aes.o

subbytes_lut:
	$(CC) -c subbytes_lut.c

mixcolumns_lut:
	$(CC) -c mixcolumns_lut.c

# --------共通----------

aes.o:	aes.c
	$(CC) -c aes.c


.PHONY: clean
clean:
	rm -f $(BIN)/* \
		test_affine test_inverse test_multiply \
		aes.o gen_sbox.o gen_mbox02.o gen_mbox03.o \
		subbytes_lut.o mixcolumns_lut.o \
		sbox.c mbox02.c mbox03.c \
		sbox.o mbox02.o mbox03.o
