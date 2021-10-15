#!/bin/bash

cd "$(dirname "$0")"

host='st17150yy@172.16.206.1'

scp -r ../AES "${host}":~
ssh "${host}" 'cd AES; cc aes.c cipherH.o subbytes.o shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o test1.o;'
ssh "${host}" '~/AES/a.out' |tee remote_result.txt

#diff remote_result.txt test1.res.txt



