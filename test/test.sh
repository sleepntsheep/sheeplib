#!/bin/sh

rm -rf bin
mkdir -p bin
for f in *.c
do
    clang "$f" -o "./bin/$f.out"  -DSNOW_ENABLED -std=c11
    "./bin/$f.out"
done
