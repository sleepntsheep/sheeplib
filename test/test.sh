#!/bin/sh

rm -rf bin
mkdir -p bin
for f in *.c
do
    gcc "$f" -o "./bin/$f.out"  -DSNOW_ENABLED -std=c99 -Wall -g
    "./bin/$f.out"
done
