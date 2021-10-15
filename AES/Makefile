SHELL=/bin/bash
CC = /usr/bin/gcc

exp1: check_affine check_multiply check_inverse check_aes_all
	@echo done

check_aes_all: aes_exp1 
	@diff ./test1.res.txt <(./exp1)  >/dev/null || echo 'all test failed'

# 関数ごとのテスト
check_affine: test_affine
	@diff ./testAffine.res.txt <(./test_affine) >/dev/null || (echo 'affine failed' && false)

test_affine: aes.o
	$(CC) -o test_affine \
		aes.o cipherH.o subbytes.o \
		shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o testAffine.o

check_multiply: test_multiply
	@diff ./testMultiply.res.txt <(./test_multiply) >/dev/null || (echo 'multiply failed' && false)

test_multiply: aes.o
	$(CC) -o test_multiply \
		aes.o cipherH.o subbytes.o \
		shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o testMultiply.o

check_inverse: test_inverse
	@diff ./testInverse.res.txt <(./test_inverse) >/dev/null || (echo 'inverse failed' && false)

test_inverse: aes.o test_multiply
	$(CC) -o test_inverse \
		aes.o cipherH.o subbytes.o \
		shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o testInverse.o


aes_exp1: aes.o
	$(CC) -o exp1 \
			aes.o cipherH.o subbytes.o shiftrows.o \
			mixcolumns.o addroundkey.o keyexpand1.o debug.o test1.o

aes.o:	aes.c
	$(CC) -c aes.c

clean:
	rm  exp1 aes.o test_affine test_inverse test_multiply 
