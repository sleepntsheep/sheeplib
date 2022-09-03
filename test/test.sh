#!/bin/sh

rm -rf bin
mkdir -p bin
for f in *.c
do
    gcc "$f" -o "./bin/$f.out"  -DSNOW_ENABLED -lcheck
    "./bin/$f.out"
done
