#!/bin/bash

cd "$(dirname "$0")"

host="${$1}"

scp -r ../AES "${host}":~
ssh "${host}" 'cd AES; cc aes.c cipherH.o subbytes.o shiftrows.o mixcolumns.o addroundkey.o keyexpand1.o debug.o test1.o;'
ssh "${host}" '~/AES/a.out' |tee remote_result.txt




