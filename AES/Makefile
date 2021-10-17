SHELL=/bin/bash
CC = /usr/bin/gcc

# 実験1:関数ごとのテストとaes.cのテストを全て実行
exp1: check_affine check_multiply check_inverse check_aes
	@echo done

# 実験2:
exp2_all: exp2_11 exp2_12 exp2_21 exp2_22
	@echo EXP2_11
	@time ./exp2_11 >/dev/null
	@echo EXP2_12
	@time ./exp2_12 >/dev/null
	@echo EXP2_21
	@time ./exp2_21 >/dev/null
	@echo EXP2_22
	@time ./exp2_22 >/dev/null


exp2_11: aes.o gen_boxes 
	@$(CC) -o exp2_11 \
		aes.o cipherH.o  shiftrows.o \
		addroundkey.o keyexpand1.o debug.o test2.o \
		subbytes.o mixcolumns.o 

exp2_12: aes.o gen_boxes mixcolumns_lut
	@$(CC) -o exp2_12 \
		aes.o cipherH.o  shiftrows.o \
		addroundkey.o keyexpand1.o debug.o test2.o \
		subbytes.o mixcolumns_lut.o mbox02.o mbox03.o

exp2_21: aes.o gen_boxes subbytes_lut
	@$(CC) -o exp2_21 \
		aes.o cipherH.o  shiftrows.o \
		addroundkey.o keyexpand2.o debug.o test2.o \
		subbytes_lut.o sbox.o mixcolumns.o 

exp2_22: aes.o gen_boxes subbytes_lut mixcolumns_lut
	@$(CC) -o exp2_22 \
		aes.o cipherH.o  shiftrows.o \
		addroundkey.o keyexpand2.o debug.o test2.o \
		subbytes_lut.o sbox.o mbox02.o mixcolumns_lut.o mbox03.o

gen_boxes: gen_sbox gen_mbox02 gen_mbox03
	@./gen_sbox > sbox.c
	@./gen_mbox02 > mbox02.c
	@./gen_mbox03 > mbox03.c
	@$(CC) -c sbox.c
	@$(CC) -c mbox02.c
	@$(CC) -c mbox03.c

# --------実験1---------
# 実験1:aes.c全体のテスト
check_aes: aes_exp1
	@diff ./test1.res.txt <(./exp1)  >/dev/null || echo 'ALL TEST FAILED'
	@echo AES PASSED

# 実験1:関数ごとのテスト
check_affine: test_affine
	@diff ./testAffine.res.txt <(./test_affine) >/dev/null || (echo 'AFFINE FAILED' && false)
	@echo AFFINE PASSED

test_affine: aes_exp1
	@$(CC) -o test_affine \
		aes.o cipherH.o subbytes.o \
		shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o testAffine.o

check_multiply: test_multiply
	@diff ./testMultiply.res.txt <(./test_multiply) >/dev/null || (echo 'MULTIPLY FAILED' && false)
	@echo MULTIPLY PASSED

test_multiply: aes_exp1
	@$(CC) -o test_multiply \
		aes.o cipherH.o subbytes.o \
		shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o testMultiply.o

check_inverse: test_inverse
	@diff ./testInverse.res.txt <(./test_inverse) >/dev/null || (echo 'INVERSE FAILED' && false)
	@echo INVERSE PASSED

test_inverse: aes_exp1
	@$(CC) -o test_inverse \
		aes.o cipherH.o subbytes.o \
		shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o testInverse.o

aes_exp1: aes.o
	$(CC) -o exp1 \
		aes.o cipherH.o subbytes.o shiftrows.o \
		mixcolumns.o addroundkey.o keyexpand1.o debug.o test1.o


# --------実験2---------
gen_sbox: gen_sbox.o aes.o
	$(CC) -o gen_sbox \
		gen_sbox.o aes.o

gen_mbox02: gen_mbox02.o aes.o
	$(CC) -o gen_mbox02 \
 		gen_mbox02.o aes.o

gen_mbox03: gen_mbox03.o aes.o
	$(CC) -o gen_mbox03 \
 		gen_mbox03.o aes.o

subbytes_lut:
	$(CC) -c subbytes_lut.c

mixcolumns_lut:
	$(CC) -c mixcolumns_lut.c

# --------共通----------
aes.o:	aes.c
	$(CC) -c aes.c


clean:
	rm -f exp1 exp2_11 exp2_12 exp2_21 exp2_22 \
		gen_sbox gen_mbox02 gen_mbox03 \
		test_affine test_inverse test_multiply \
		aes.o gen_sbox.o gen_mbox02.o gen_mbox03.o \
		subbytes_lut.o mixcolumns_lut.o \
		sbox.c mbox02.c mbox03.c \
		sbox.o mbox02.o mbox03.o
