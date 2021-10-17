SHELL=/$(BIN)/bash
CC = /usr/$(BIN)/gcc
BIN = bin
basefiles = aes.o cipherH.o shiftrows.o  addroundkey.o debug.o 
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
check_io_exp2: to_test1  exp2_11 exp2_12 exp2_21 exp2_22
	@diff ./test1.res.txt <($(BIN)/exp2_11)  >/dev/null || (echo 'EXP2_11 FAILED' && false)
	@echo 1 1 passed
	@diff ./test1.res.txt <($(BIN)/exp2_12)  >/dev/null || (echo 'EXP2_12 FAILED' && false)
	@echo 1 2 passed
	@diff ./test1.res.txt <($(BIN)/exp2_21)  >/dev/null || (echo 'EXP2_21 FAILED' && false)
	@echo 2 1 passed
	@diff ./test1.res.txt <($(BIN)/exp2_22)  >/dev/null || (echo 'EXP2_22 FAILED' && false)
	@echo 2 2 passed
	
# タイム計測
measure_exp2_all: to_test2 exp2_11 exp2_12 exp2_21 exp2_22
	@echo EXP2_11
	@time $(BIN)/exp2_11 >/dev/null
	@echo EXP2_12
	@time $(BIN)/exp2_12 >/dev/null
	@echo EXP2_21
	@time $(BIN)/exp2_21 >/dev/null
	@echo EXP2_22
	@time $(BIN)/exp2_22 >/dev/null



# 実験2:関数の組み合わせを変えた実行ファイルのコンパイル
exp2_11: aes.o gen_boxes 
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
		subbytes_lut.o sbox.o mbox02.o mixcolumns_lut.o mbox03.o \
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
check_aes: to_test1 aes_exp1
	@diff ./test1.res.txt <($(BIN)/exp1)  >/dev/null || echo 'ALL TEST FAILED'
	@echo AES PASSED

## 実験1:関数ごとのテスト
check_affine: to_test1 test_affine
	@diff ./testAffine.res.txt <($(BIN)/test_affine) >/dev/null || (echo 'AFFINE FAILED' && false)
	@echo AFFINE PASSED

check_multiply: to_test1 test_multiply
	@diff ./testMultiply.res.txt <($(BIN)/test_multiply) >/dev/null || (echo 'MULTIPLY FAILED' && false)
	@echo MULTIPLY PASSED


check_inverse: to_test1 test_inverse
	@diff ./testInverse.res.txt <($(BIN)/test_inverse) >/dev/null || (echo 'INVERSE FAILED' && false)
	@echo INVERSE PASSED


# 実験1:関数ごとのテスト実行ファイルのコンパイル
test_affine: aes_exp1
	$(CC) -o $(BIN)/test_affine \
		aes.o cipherH.o subbytes.o \
		shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o testAffine.o

test_multiply: aes_exp1
	$(CC) -o $(BIN)/test_multiply \
		aes.o cipherH.o subbytes.o \
		shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o testMultiply.o

test_inverse: aes_exp1
	$(CC) -o $(BIN)/test_inverse \
		aes.o cipherH.o subbytes.o \
		shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o testInverse.o

# 実験1用の実行ファイル
aes_exp1: aes.o
	$(CC) -o $(BIN)/exp1 \
		$(basefiles) \
		subbytes.o  \
		mixcolumns.o keyexpand1.o $(TESTER)





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


clean:
	rm -f $(BIN)/* \
		test_affine test_inverse test_multiply \
		aes.o gen_sbox.o gen_mbox02.o gen_mbox03.o \
		subbytes_lut.o mixcolumns_lut.o \
		sbox.c mbox02.c mbox03.c \
		sbox.o mbox02.o mbox03.o
